#include <string>

#include "Histogram.hpp"
#include "Log.hpp"
#include "PageContainer.hpp"
#include "StatSender.hpp"
#include "UsedMemory.hpp"
#include "boost/program_options.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  po::options_description desc("Options:");

  desc.add_options()("help", ": HELPER")("threshold", po::value<float>(),
                                         ": PARAMETR OF THRESHOPLD")(
      "filename", po::value<std::string>(), ": IS NAME OF FILE");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  float threshold;
  if(vm.count("threshold")){
    threshold = vm["threshold"].as<float>();
  }
  else{
    threshold = 0;
  }

  std::string filename;
  if(vm.count("filename")){
    filename = vm["filename"].as<std::string>();
  }
  else{
    filename = "../misc/data.txt";
  }


  PageContainer page;
  StatSender stat(page);
  UsedMemory used_memory(page);
  Histogram histogram(page);

  std::ifstream in(filename);

  try {
    page.Load(in, threshold);
  } catch (const std::runtime_error& e) {
    std::cout << e.what() << std::endl;
  }
}