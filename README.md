# Angiogenesis3D1D
3D-1D tumor growth model for simulation of angiogenesis 

### Petsc solver types
We can select the following solvers (and more can be found in the petsc website) with the option `-ksp_type`
	- cg - Conjugate Gradient
	- gmres - Generalized Minimal Residual method
	- bcgs - Stabilized version of BiConjugate Gradient
	- tfqmr - A transpose free QMR (quasi minimal residual)
	- cgs - Conjugate Gradient Squared
