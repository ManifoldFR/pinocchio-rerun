import pinocchio_rerun
import example_robot_data as erd
import pinocchio as pin
import numpy as np


robot = erd.load("ur5")
model = robot.model
visual_model = robot.visual_model
rr = pinocchio_rerun.RerunVisualizer(model, visual_model)
rr.loadViewerModel()
q0 = pin.neutral(model)
rr.display(q0)

T = 80
dt = 0.01
qs = np.zeros((T + 1, model.nq))
vs = np.zeros((T + 1, model.nv))
qs[0] = q0

for t in range(0, T):
    q, v = qs[t], vs[t]
    tau = pin.rnea(model, rr.data, q, v, np.zeros(model.nv))
    a = pin.aba(model, rr.data, q, v, 0.9 * tau)
    vp = v + dt * a
    qp = pin.integrate(model, q, vp * dt)

    qs[t + 1] = qp
    vs[t + 1] = vp


frame_id0 = model.getFrameId("elbow_joint")
frame_id1 = model.getFrameId("tool0")
frame_ids = [frame_id0, frame_id1]


for t in range(0, T + 1):
    q, v = qs[t], vs[t]
    pin.forwardKinematics(model, rr.data, q, v)
    rr.display()
    rr.drawFrameVelocities(frame_ids)

rr.play(list(qs), dt)
