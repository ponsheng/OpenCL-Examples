__kernel void pipe_writer(__global int *in, write_only pipe int p_in) {
  int gid = get_global_id(0);
  write_pipe(p_in, &in[gid]);
}

#define RESV_CNT 5

__kernel void pipe_reader(__global int *out, read_only pipe int p_out) {
  int gid = get_global_id(0);
  reserve_id_t res_id;

  res_id = reserve_read_pipe(p_out, RESV_CNT);
  if (is_valid_reserve_id(res_id)) {
    read_pipe(p_out, res_id, 3, &out[gid * RESV_CNT + 3]);
    read_pipe(p_out, res_id, 2, &out[gid * RESV_CNT + 2]);
    read_pipe(p_out, res_id, 1, &out[gid * RESV_CNT + 1]);
    read_pipe(p_out, res_id, 0, &out[gid * RESV_CNT + 0]);
    commit_read_pipe(p_out, res_id);
  }
}
