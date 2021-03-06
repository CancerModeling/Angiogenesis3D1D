import sys

from seeds import seeds
from config._default_sim_params  import TwoVesselsSimParams as DefaultSimParams
from config._setup_directories import setup_directories 
from config._call_executibles import call_executibles 


class SimParams(DefaultSimParams):
    def __init__(self):
        super(SimParams, self).__init__()
        # Play with parameters here... 


def setup_params():
  num_elems = 55
  dps = []
  scale = 0.0025
  lower_bound = 0.1
  upper_bound = 0.9
  num_eigenfunctions = 17

  num_samples = 1
  
  for i in range(0, num_samples):
    # create sim params
    dp = SimParams()
    dp.num_elems = num_elems
    dp.E_phi_T = 0.03

    dp.pp_tag = 'twoves_angio_explicit_iter3d1d_noise_elems_{}_sample_{}'.format(num_elems, i)
    dp.seed = seeds[i]
    dp.lambda_TAF_deg = 0
    dp.sigma_HTAF = 0
    dp.delta_t = 0.0025
    dp.dt_output = 32
    dp.final_time = 8.

    dp.run_path = 'sim/' + dp.pp_tag + '/'
    dp.network_update_interval = 2
    dp.network_sprout_prob = 0.4

    dp.hyp_noise_num_eigenfunctions = num_eigenfunctions
    dp.hyp_noise_seed = dp.seed
    dp.hyp_noise_scale = scale
    dp.hyp_noise_lower_bound = lower_bound
    dp.hyp_noise_upper_bound = upper_bound

    dp.pro_noise_num_eigenfunctions = num_eigenfunctions
    dp.pro_noise_seed = dp.seed + 1
    dp.pro_noise_scale = scale
    dp.pro_noise_lower_bound = lower_bound
    dp.pro_noise_upper_bound = upper_bound

    dp.scheme_name = 'solve_explicit'

    #dp.coupled_1d3d = True
    #dp.n_mpi = 0

    dp.coupled_1d3d = False
    dp.n_mpi = 4

    dp.k_WSS = 0.40
    dp.k_s = 0.12
    dp.offset_tau = 0.02

    # dp.network_update_interval = 1000000000

    dps.append(dp)
  return dps



if len(sys.argv) > 1:
  if str(sys.argv[1]) == 'run':
    dps = setup_params()
    setup_directories(dps)
    call_executibles(dps, run_screen=False)
  elif str(sys.argv[1]) == 'run_with_screen':
    dps = setup_params()
    setup_directories(dps)
    call_executibles(dps, run_screen=True)
  elif str(sys.argv[1]) == 'prepare':
    dps = setup_params()
    setup_directories(dps)
  else:
    print('?')
else:
    print('To run sim, run script with argument "run"')

