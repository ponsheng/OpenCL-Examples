__kernel void pipe_writer(__global int *in, write_only pipe int p_in) {
  int gid = get_global_id(0);
  write_pipe(p_in, &in[gid]);
}

__kernel void pipe_reader(__global int *out, read_only pipe int p_out) {
  int gid = get_global_id(0);
  reserve_id_t res_id;

  res_id = reserve_write_pipe(p_out, 4);
  if (is_valid_reserve_id(res_id)) {
    read_pipe(p_out, &out[gid * 4]);
    read_pipe(p_out, &out[gid * 4 + 1]);
    read_pipe(p_out, &out[gid * 4 + 2]);
    read_pipe(p_out, &out[gid * 4 + 3]);
  }
}
