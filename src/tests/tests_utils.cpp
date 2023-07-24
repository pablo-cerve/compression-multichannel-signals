
#include "tests_utils.h"
#include "os_utils.h"

const std::string TestsUtils::DATASETS_PATH = OSUtils::DATASETS_CSV_PATH;
const std::string TestsUtils::NOAA_SPC_PATH = DATASETS_PATH + "/[6]noaa-spc-reports";

const std::string TestsUtils::IRKIS_PATH = DATASETS_PATH + "/[1]irkis";
const std::string TestsUtils::NOAA_SST_PATH = DATASETS_PATH + "/[2]noaa-sst/months/2017";
const std::string TestsUtils::NOAA_ADCP_PATH = DATASETS_PATH + "/[3]noaa-adcp/2015";
const std::string TestsUtils::SOLAR_ANYWHERE_PATH = DATASETS_PATH + "/[4]solar-anywhere/all";
const std::string TestsUtils::EL_NINO_PATH = DATASETS_PATH + "/[5]el-nino";
const std::string TestsUtils::NOAA_SPC_HAIL_PATH = NOAA_SPC_PATH + "/hail";
const std::string TestsUtils::NOAA_SPC_TORNADO_PATH = NOAA_SPC_PATH + "/tornado";
const std::string TestsUtils::NOAA_SPC_WIND_PATH = NOAA_SPC_PATH + "/wind";

const std::string TestsUtils::OUTPUT_PATH = OSUtils::CPP_PROJECT_PATH + "/test_files";
