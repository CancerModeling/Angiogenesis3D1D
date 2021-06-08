#!/bin/bash
MY_PWD=$(pwd)

## Fixed variable which should be changed less often
EXEC_DIR=`pwd`/../build/bin/

## libmesh options
LIBMESH_OPTS2="--solver-system-names "
LIBMESH_OPTS2+="-prolific_ksp_type gmres -prolific_pc_type fieldsplit -prolific_pc_fieldsplit_type schur -prolific_pc_fieldsplit_schur_fact_type lower "
LIBMESH_OPTS2+="-hypoxic_ksp_type gmres -hypoxic_pc_type fieldsplit -hypoxic_pc_fieldsplit_type schur -hypoxic_pc_fieldsplit_schur_fact_type lower "
LIBMESH_OPTS2+="-nutrient_ksp_type gmres -nutrient_pc_type bjacobi -nutrient_sub_pc_type ilu -nutrient_sub_pc_factor_levels 0 "
LIBMESH_OPTS2+="-necrotic_ksp_type gmres -necrotic_pc_type bjacobi -necrotic_sub_pc_type ilu -necrotic_sub_pc_factor_levels 0 "
LIBMESH_OPTS2+="-taf_ksp_type gmres -taf_pc_type bjacobi "
LIBMESH_OPTS2+="-ecm_ksp_type gmres -ecm_pc_type bjacobi "
LIBMESH_OPTS2+="-mde_ksp_type gmres -mde_pc_type bjacobi "

LIBMESH_OPTS="-ksp_type gmres -pc_type bjacobi -sub_pc_type ilu -sub_pc_factor_levels 0 "

model_name="$1"
sim_dir="$2"
n_mpi="$3"
run_screen="$4"
pp_tag=$5

(

# if sim dir exist, clean it first
if [[ -d $sim_dir ]]; then
	rm $sim_dir/*vtu > /dev/null 2>&1
	rm $sim_dir/*txt > /dev/null 2>&1
	rm $sim_dir/*vtk > /dev/null 2>&1
	rm $sim_dir/*log > /dev/null 2>&1
fi

cd $sim_dir

echo "$pwd"

if [[ $n_mpi == "0" ]]; then

    if [[ $run_screen == "0" ]]; then
        "$EXEC_DIR/$model_name" input.in $LIBMESH_OPTS
    else
        screen -dmS $pp_tag-$model_name "$EXEC_DIR/$model_name" input.in $LIBMESH_OPTS
    fi
else

    if [[ $run_screen == "0" ]]; then
        mpiexec -np $n_mpi "$EXEC_DIR/$model_name" input.in $LIBMESH_OPTS
    else
        screen -dmS $pp_tag-$model_name mpiexec -np $n_mpi "$EXEC_DIR/$model_name" input.in $LIBMESH_OPTS
	echo 'starting in shell ' + ${pp_tag-$model_name}
    fi
fi

) |& tee "$sim_dir/sim.log"
