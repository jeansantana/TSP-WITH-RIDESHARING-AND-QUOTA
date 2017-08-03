mkdir results && mkdir results/gen && mkdir results/mem && mkdir results/gen/splitop && mkdir results/gen/scxop && mkdir results/gen/scxbasedop && mkdir results/mem/splitop && mkdir results/mem/scxop && mkdir results/mem/scxbasedop

python a_run_gen_split.py build/METAHEURISTIC ../instances/paper/experiment/all/simetric/ results/gen/splitop/ 30 &
python b_run_gen_scx.py build/METAHEURISTIC ../instances/paper/experiment/all/simetric/ results/gen/scxop/ 30 &
python c_run_gen_scx_based.py build/METAHEURISTIC ../instances/paper/experiment/all/simetric/ results/gen/scxbasedop/ 30 &

python d_run_mem_split.py build/METAHEURISTIC ../instances/paper/experiment/all/simetric/ results/mem/splitop/ 30 &
python e_run_mem_scx.py  build/METAHEURISTIC ../instances/paper/experiment/all/simetric/ results/mem/scxop/ 30 &
python f_run_mem_scx_based.py build/METAHEURISTIC ../instances/paper/experiment/all/simetric/ results/mem/scxbasedop/ 30 &