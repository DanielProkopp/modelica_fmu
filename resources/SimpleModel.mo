model SimpleModel "Simple Model for testing"
  input Real x;
  output Real y "y output";
equation
  y = 2*x;
end SimpleModel;
