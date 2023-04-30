import pinocchio as pin
import numpy as np
import sys
import os
from os.path import dirname, join, abspath

from pinocchio.visualize import GepettoVisualizer

# Load the URDF model.
pinocchio_model_dir = join(dirname(abspath(__file__)), "../models")

model_path = join(pinocchio_model_dir, 'example-robot-data/robots/talos_data/robots')
print(model_path)
mesh_dir = pinocchio_model_dir
print(mesh_dir)
urdf_filename = "talos_reduced.urdf"
urdf_model_path = os.path.join(model_path, urdf_filename)

model, collision_model, visual_model = pin.buildModelsFromUrdf(urdf_model_path, mesh_dir, pin.JointModelFreeFlyer())
viz = GepettoVisualizer(model, collision_model, visual_model)

#Initialize the viewer.
try:
    viz.initViewer()
except ImportError as err:
    print("Error while initializing the viewer. It seems you should install gepetto-viewer.")
    print(err)
    sys.exit(0)

try:
    viz.loadViewerModel("pinocchio")
except AttributeError as err:
	print("Error while loading the viewer model.")
	print(err)
	sys.exit(0)

#Display a robot configuration.
q0 = pin.neutral(model)
viz.display(q0)

#Display another robot.
viz2 = GepettoVisualizer(model, collision_model, visual_model)
viz2.initViewer(viz.viewer)
viz2.loadViewerModel("pinocchio2")
q = q0.copy()
q[1] = 1.0
viz2.display(q) 
