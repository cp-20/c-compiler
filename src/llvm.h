#pragma once

typedef int* rctx;

int* r_init(void);

void r_free(rctx rctx);

int r_register(rctx rctx);

int* r_register_ptr(rctx rctx);
