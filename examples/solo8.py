import pyseptum
import example_robot_data as erd
import pinocchio as pin
import numpy as np


robot = erd.load("solo8")
model = robot.model
visual_model = robot.visual_model
rr = pyseptum.RerunVisualizer(model, visual_model)
rr.initViewer()
assert rr.initialized
q0 = pin.neutral(model)
rr.display(q0)
rr.display()


def displayRandomConfiguration():
    q = pin.randomConfiguration(model)
    q[:3] = np.clip(q[:3], -1., 1.)
    rr.display(q)
