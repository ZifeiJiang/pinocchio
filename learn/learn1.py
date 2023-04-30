import pinocchio
from sys import argv
from os.path import dirname, join, abspath

pinocchio_model_dir = join(dirname(abspath(__file__)), "../models")

urdf_filename = pinocchio_model_dir + '/example-robot-data/robots/ur_description/urdf/ur5_gripper.urdf' if len(argv)<2 else argv[1]

model = pinocchio.buildModelFromUrdf(urdf_filename)
print('model name: ', model.name)

data = model.createData()

q = pinocchio.randomConfiguration(model)
print('q: ', q.T)

pinocchio.forwardKinematics(model, data, q)

for name, oMi in zip(model.names, data.oMi):
	print(name, 'placement: ', oMi)