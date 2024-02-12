#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <clipp.h>


#include <fmi4cpp/fmi4cpp.hpp>

using namespace fmi4cpp;

constexpr double stop = 0.0;
constexpr double stepSize = 0.01;

std::vector<fmi2Real> run_simulation(double stop, double step, double x_val)
{
  std::vector<fmi2Real> var_y(1);
  std::vector<fmi2Real> var_x{x_val};
  const std::string fmu_path = "../resources/SimpleModel.fmu";
  
  fmi2::fmu fmu(fmu_path);
  
  const auto slave = fmu.as_cs_fmu()->new_instance();
  
  auto model_desc = slave->get_model_description();

  std::cout << "model_identifier: " << model_desc->model_identifier << "\n";

  slave->setup_experiment();
  slave->enter_initialization_mode();
  slave->exit_initialization_mode();

  // reference for writing
    std::vector<fmi2ValueReference> vr_x = {
    model_desc->get_variable_by_name("x").value_reference,
  };
  // reference of result
  std::vector<fmi2ValueReference> vr_y = {
    model_desc->get_variable_by_name("y").value_reference,
  };

  double t;
  while((t = slave->get_simulation_time()) <= stop)
  {
    if(!slave->step(stepSize))
      break;
    if(!slave->write_real(vr_x, var_x))
      break;
    if(!slave->read_real(vr_y, var_y))
      break;
    // std::cout << "Y=" << var_y[0] << "\n";
  }
  
  std::cout << "FMU '" << fmu.model_name() << 
    "' terminated with success: " << std::boolalpha << slave->terminate() << "\n";
  return var_y;
}


int main(int argc, char *argv[]) {
  std::string x_str;
  std::string step_str;
  std::string stop_str;

  auto cli = (
    clipp::required("-x") & clipp::value("X variable", x_str),
    clipp::required("-s") & clipp::value("Simulation step", step_str),
    clipp::required("-S") & clipp::value("Simulation stop time", stop_str)
  ); 

  if(!clipp::parse(argc, argv, cli)){
    std::cerr << clipp::make_man_page(cli, argv[0]);
    return EXIT_FAILURE;
  }

  double x = std::stof(x_str);
  double step = std::stof(step_str);
  double stop = std::stoi(stop_str);

  auto result = run_simulation(stop, step, x);
  std::cout << "=================================================" << std::endl;
  std::cout << "Inputs:\n";
  std::cout << "x=" << x << "\n";
  std::cout << "step=" << step << "\n";
  std::cout << "stop=" << stop << "\n";
  std::cout << "=================================================" << std::endl;
  std::cout << "Results: \n";
  for(auto r : result)
    std::cout << "Y=" << r << "\n";
  return 0;
}
