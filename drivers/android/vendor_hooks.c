// SPDX-License-Identifier: GPL-2.0-only
/* vendor_hook.c
 *
 * Android Vendor Hook Support
 *
 * Copyright 2020 Google LLC
 */

#define CREATE_TRACE_POINTS
#include <trace/hooks/vendor_hooks.h>
#include <trace/hooks/memory.h>
#include <trace/hooks/syscall_check.h>

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_x);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_nx);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_ro);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_rw);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_mmap_file);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_file_open);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_bpf_syscall);
