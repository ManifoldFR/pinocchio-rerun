import pinocchio_rerun
import example_robot_data as erd
import pinocchio as pin
import numpy as np


robot = erd.load("talos")
model = robot.model
visual_model = robot.visual_model
rr = pinocchio_rerun.RerunVisualizer(model, visual_model)
rr.initViewer()
assert rr.initialized
q0 = pin.neutral(model)
rr.display(q0)
rr.display()

def displayRandomConfiguration():
    q = pin.randomConfiguration(model)
    q[:3] = np.clip(q[:3], -1., 1.)
    rr.display(q)
