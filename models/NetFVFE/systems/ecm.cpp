////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2019 Prashant K. Jha
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include "../model.hpp"

Number netfvfe::initial_condition_ecm(const Point &p, const Parameters &es,
                                      const std::string &system_name, const std::string &var_name) {

  libmesh_assert_equal_to(system_name, "ECM");

  if (var_name == "ecm") {

    const auto *deck = es.get<InpDeck *>("input_deck");
    const auto &ic_data = deck->d_ecm_ic_data;

    return ic_data.d_val;
  }

  return 0;
}

// Assembly class
void netfvfe::EcmAssembly::assemble() {
  assemble_1();
}

void netfvfe::EcmAssembly::assemble_1() {

  // Get required system alias
  auto &ecm = d_model_p->get_ecm_assembly();
  auto &nut = d_model_p->get_nut_assembly();
  auto &mde = d_model_p->get_mde_assembly();

  // Model parameters
  const auto &deck = d_model_p->get_input_deck();
  const Real dt = d_model_p->d_dt;

  // Store current and old solution
  Real ecm_old = 0.;
  Real ecm_cur = 0.;
  Real nut_cur = 0.;
  Real mde_cur = 0.;
  Real mde_old = 0.;

  Real compute_rhs = 0.;
  Real compute_mat = 0.;

  // Looping through elements
  for (const auto &elem : d_mesh.active_local_element_ptr_range()) {

    ecm.init_dof(elem);
    nut.init_dof(elem);
    mde.init_dof(elem);

    // init fe and element matrix and vector
    ecm.init_fe(elem);

    // get finite-volume quantities
    nut_cur = nut.get_current_sol(0);

    for (unsigned int qp = 0; qp < d_qrule.n_points(); qp++) {

      if (d_implicit_assembly) {
        // required
        // old: ecm
        // new: nut, mde, ecm
        ecm_old = 0.;
        mde_cur = 0.;
        ecm_cur = 0.;
        for (unsigned int l = 0; l < d_phi.size(); l++) {

          ecm_cur += d_phi[l][qp] * ecm.get_current_sol(l);
          ecm_old += d_phi[l][qp] * ecm.get_old_sol(l);
          mde_cur += d_phi[l][qp] * mde.get_current_sol(l);
        }

        compute_rhs =
          d_JxW[qp] *
          (ecm_old + dt * deck.d_lambda_ECM_P * nut_cur *
                       util::heaviside(ecm_cur - deck.d_bar_phi_ECM_P));

        compute_mat =
          d_JxW[qp] * (1. + dt * deck.d_lambda_ECM_D * mde_cur +
                       dt * deck.d_lambda_ECM_P * nut_cur *
                         util::heaviside(ecm_cur - deck.d_bar_phi_ECM_P));

      } else {
        // required
        // old: ecm, mde
        // new: nut
        ecm_old = 0.;
        mde_old = 0.;
        for (unsigned int l = 0; l < d_phi.size(); l++) {

          ecm_old += d_phi[l][qp] * ecm.get_old_sol(l);
          mde_old += d_phi[l][qp] * mde.get_old_sol(l);
        }

        compute_rhs =
          d_JxW[qp] *
          (ecm_old + dt * deck.d_lambda_ECM_P * nut_cur * util::heaviside(ecm_old - deck.d_bar_phi_ECM_P) * (1. - ecm_old) - dt * deck.d_lambda_ECM_D * mde_old * ecm_old);

        compute_mat = d_JxW[qp];
      }

      // Assembling matrix
      for (unsigned int i = 0; i < d_phi.size(); i++) {

        d_Fe(i) += compute_rhs * d_phi[i][qp];

        for (unsigned int j = 0; j < d_phi.size(); j++) {

          d_Ke(i, j) += compute_mat * d_phi[j][qp] * d_phi[i][qp];
        }
      }
    } // loop over quadrature points

    d_dof_map_sys.heterogenously_constrain_element_matrix_and_vector(d_Ke, d_Fe,
                                                                     d_dof_indices_sys);
    d_sys.matrix->add_matrix(d_Ke, d_dof_indices_sys);
    d_sys.rhs->add_vector(d_Fe, d_dof_indices_sys);
  }
}
