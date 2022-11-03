__kernel void pipe_writer(__global float *in, write_only pipe float p_in) {
  int gid = get_global_id(0);
  write_pipe(p_in, &in[gid]);
}

__kernel void pipe_reader(__global float *out, read_only pipe float p_out) {
  int gid = get_global_id(0);
  read_pipe(p_out, &out[gid]);
}
