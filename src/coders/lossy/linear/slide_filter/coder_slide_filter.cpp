
#include "coder_slide_filter.h"
#include "slide_filter_window.h"
#include "assert.h"
#include "math_utils.h"
#include <iomanip>
#include "coder_utils.h"

void CoderSlideFilter::setCoderParams(int window_size_, std::vector<int> epsilons_vector_){
    window_size = window_size_;
    epsilons_vector = epsilons_vector_;
}

void CoderSlideFilter::codeColumnBefore(){
    delta_sum = 0;
    int epsilon = getEpsilon(epsilons_vector, column_index);
    data = new SlideFilterWindow(total_data_rows, epsilon);
}

void CoderSlideFilter::codeColumnWhile(std::string csv_value){
    int delta = time_delta_vector[row_index]; // >= 0
    if (Constants::isNoData(csv_value)) {
        delta_sum += delta; // delta >= 0
        return; // skip no_data
    }
    delta_sum += CoderUtils::calculateDelta(delta, row_index);
    data->addDataItem(delta_sum, csv_value);
    delta_sum = 0;
}

void CoderSlideFilter::codeColumnAfter() {
    assert(data->length == total_data_rows);
    if (total_data_rows > 0){
        compress();
    }
    delete data;
}

void CoderSlideFilter::codeEntry(bool connToFollow, double timestamp, double value){
    codeBool(connToFollow);
    codeFloat(timestamp);
    codeFloat(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CoderSlideFilter::compress(){
    int inputSize = data->getDataLength();
    int i = 0;
    int epsilon = getEpsilon(epsilons_vector, column_index);

    while(i < inputSize) {
        int win_size = ((inputSize - i) < window_size) ? (inputSize - i) : window_size;
        m_pSFData = new SlideFilterWindow(win_size, epsilon);
        int first_timestamp = data->getAt(i).timestamp;
        for(int j = i; j < i + win_size; j++){
            DataItem item2 = data->getAt(j);
            int diff_timestamp = item2.timestamp - first_timestamp; // the first timestamp is always 0
            m_pSFData->addDataItemTwo(diff_timestamp, item2.value);
        }
        compressWindow();
        delete m_pSFData;
        i += win_size;
    }
}


// compress raw data
void CoderSlideFilter::compressWindow()
{
    m_nBegin_Point = 0;
    int inputSize = m_pSFData->getDataLength();
    if (inputSize == 1)
    {
        DataItem item = m_pSFData->getAt(0);
        codeEntry(true, item.timestamp, item.value);
        m_nBegin_Point = 1;
        return;
    }

    // Initialize upper bound and lower bound
    double eps = m_pSFData->getEsp();
    DataItem item1 = m_pSFData->getAt(0);
    DataItem item2 = m_pSFData->getAt(1);
    initializeU_L(item1.timestamp, item1.value, item2.timestamp, item2.value, eps);

    //Read the input
    double upperValue, lowerValue;
    DataItem item;
    for (int i = 2; i <= inputSize; i++)
    {
        //Read if it is not the end of the input
        if (i < inputSize)
        {
            item = m_pSFData->getAt(i);
            upperValue = m_curU.getValue(item.timestamp);
            lowerValue = m_curL.getValue(item.timestamp);
        }

        //recording mechanism
        if ((i == inputSize) || (item.value - upperValue > eps) || (lowerValue - item.value > eps))
        {
            recording_mechanism(i);
        }
        else //filtering mechanism
        {
            filtering_mechanism(i);
        }
    }
}

// Initialize upper bound and lower bound
void CoderSlideFilter::initializeU_L(double t1, double v1, double t2, double v2, double eps)
{
    Point top1(v1 + eps, t1);
    Point bottom1(v1 - eps, t1);
    Point top2(v2 + eps, t2);
    Point bottom2(v2- eps, t2);
    m_curL.update(&top1, &bottom2);
    m_curU.update(&bottom1, &top2);
}

// Update upper bound and lower bound
bool CoderSlideFilter::updateUandLforConnectedSegment(Line& curU, Line& curL, Line prevG)
{
    if (m_nBegin_Point ==0)
        return false;

    Point ul = curU.getIntersection(&curL);
    DataItem preItem= m_pSFData->getAt(m_nBegin_Point - 1);
    DataItem firstItem = m_pSFData->getAt(m_nBegin_Point);

    // Compute alpha, beta
    Point alpha(prevG.getValue(preItem.timestamp), preItem.timestamp);
    Point beta(prevG.getValue(firstItem.timestamp), firstItem.timestamp);
    Line alpha_ul(&ul,&alpha);
    Line beta_ul(&ul, &beta);

    double minSlope, maxSlope;
    if (alpha_ul.getSlope() < beta_ul.getSlope())
    {
        minSlope = alpha_ul.getSlope();
        maxSlope = beta_ul.getSlope();
    }
    else
    {
        maxSlope = alpha_ul.getSlope();
        minSlope = beta_ul.getSlope();
    }

    // Check whether current segment is connected with previous 'prevG'
    bool isConnected = false;
    if (minSlope < curU.getSlope() && maxSlope > curL.getSlope())
    {
        Point ul = curU.getIntersection(&curL);
        isConnected = true;

        // Update bounds
        if (maxSlope < curU.getSlope())
        {
            curU.update(&ul,maxSlope);
        }
        if (minSlope > curL.getSlope())
        {
            curL.update(&ul,minSlope);
        }
    }

    return isConnected;
}

// Find the fittest line in range of lower bound and upper bound
Line CoderSlideFilter::getFittestLine_G(int beginPoint, int endPoint, Line curU, Line curL)
{
    Point ul = curU.getIntersection(curL);
    double numberator = 0;
    double denominator = 0;

    for (int j = beginPoint + 1; j < endPoint;  j++)
    {
        DataItem item = m_pSFData->getAt(j);
        numberator += (item.value - ul.y) * (item.timestamp - ul.x);
        denominator += (item.timestamp - ul.x) * (item.timestamp - ul.x);
    }

    double fraction = numberator / denominator;
    double slopeU = curU.getSlope();
    double slopeL = curL.getSlope();

    // Get fittest slope in range of lower and upper bounds
    double fittestSlope = fraction > slopeU ? slopeU : fraction;
    fittestSlope = fittestSlope > slopeL ? fittestSlope : slopeL;

    // Create fittest line
    Line fittestLine;
    fittestLine.setSlope(fittestSlope);
    fittestLine.setIntercept(ul.y - (fittestSlope * ul.x));
    return fittestLine;
}

// Generate line segments for the filtering intervals and update the latest-executed point
void CoderSlideFilter::recording_mechanism(int& position)
{
    int inputSize = m_pSFData->getDataLength();
    m_curU.getIntersection(m_curL);
    DataItem begin_curSeg = m_pSFData->getAt(m_nBegin_Point);

    bool existInter = updateUandLforConnectedSegment(m_curU,m_curL,m_prevG);
    m_curG = getFittestLine_G(m_nBegin_Point, position, m_curU, m_curL);

    if (m_nBegin_Point == 0)
    {
        //Create first recording
        double t = begin_curSeg.timestamp;
        codeEntry(true, t, m_curG.getValue(t));
    }
    else if (existInter && m_pSFData->getEsp() > 0)
    {
        //m_curG cut m_prevG at valid section
        Point inter = m_curG.getIntersection(m_prevG);
        codeEntry(existInter, inter.x, inter.y);
    }
    else
    {
        //m_curG cut m_prevG at invalid section
        DataItem end_prevSeg = m_pSFData->getAt(m_nBegin_Point - 1);
        double t = end_prevSeg.timestamp;
        codeEntry(existInter, t, m_prevG.getValue(t));
        t = begin_curSeg.timestamp;
        codeEntry(true, t, m_curG.getValue(t));
    }

    if (position < inputSize -1)
    {
        //Create new interval by two points
        m_nBegin_Point = position;
        DataItem curItem = m_pSFData->getAt(position);
        position++;
        DataItem nextItem = m_pSFData->getAt(position);
        double eps = m_pSFData->getEsp();
        initializeU_L(curItem.timestamp, curItem.value, nextItem.timestamp, nextItem.value, eps);
        m_prevG = m_curG;
    }
    //if last interval has only one point --> Create last recording and finish compressing
    else if (position == (inputSize - 1))
    {
        m_nBegin_Point = position;
        DataItem preItem = m_pSFData->getAt(m_nBegin_Point - 1);
        DataItem item = m_pSFData->getAt(position);
        double t = preItem.timestamp;
        codeEntry(true, t, m_curG.getValue(t));
        codeEntry(false, item.timestamp, item.value);
        position++;
    }
        //position == inputSize --> Create last recording
    else
    {
        DataItem item = m_pSFData->getAt(position - 1);
        double t = item.timestamp;
        codeEntry(false, t, m_curG.getValue(t));
    }
}

// Adjust upper bound and lower bound with the arrival of each new data point at 'position'
void CoderSlideFilter::filtering_mechanism(int position)
{
    DataItem item = m_pSFData->getAt(position);
    double upperValue = m_curU.getValue(item.timestamp);
    double lowerValue = m_curL.getValue(item.timestamp);
    int j;

    double eps = m_pSFData->getEsp();
    Point top(item.value + eps, item.timestamp);
    Point bottom(item.value - eps, item.timestamp);
    DataItem tmpItem;

    //item is above L a distance which is larger than epsilon
    if (item.value - lowerValue > eps)
    {
        //Update L
        for (j = m_nBegin_Point; j < position; j++)
        {
            tmpItem = m_pSFData->getAt(j);
            Point tmpTop(tmpItem.value + eps, tmpItem.timestamp);
            Line tmpL(&tmpTop, &bottom);

            if (tmpL.getSlope() > m_curL.getSlope())
            {
                m_curL.setSlope(tmpL.getSlope());
                m_curL.setIntercept(tmpL.getIntercept());
            }
        }
    }

    //item is under U a distance which is larger than epsilon
    if (upperValue - item.value > eps)
    {
        //Update U
        for (j = m_nBegin_Point; j < position; j++)
        {
            tmpItem = m_pSFData->getAt(j);
            Point tmpBottom(tmpItem.value - eps, tmpItem.timestamp);
            Line tmpU(&tmpBottom, &top);

            if (tmpU.getSlope() < m_curU.getSlope())
            {
                m_curU.setSlope(tmpU.getSlope());
                m_curU.setIntercept(tmpU.getIntercept());
            }
        }
    }
}
