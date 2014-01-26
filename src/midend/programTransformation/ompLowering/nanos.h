/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/
#ifndef _NANOS_H_
#define _NANOS_H_
/*!
 * \file  nanos.h
 * \brief Main Nanos++ header file.
 */

/*!
 * \mainpage  Nanos++ Runtime Library
 *
 * This is the main development page for documenting Nanos++ Runtime Library (Nanos++ RTL).
 * <p/><br/>
 * Nanos++ is an extensible Runtime Library designed to serve as a runtime support in parallel environments.
 * It is mainly used to support OmpSs (an extension to the OpenMP programming model) developed at BSC but it also has modules to support OpenMP and Chapel.
 * <p/><br/>
 */

#include <unistd.h>
#include "nanos-int.h"
#include "nanos_error.h"

/*! \defgroup capi C/C++ API */
/*! \addtogroup capi
 *  \{
 */

#include "nanos_version.h"

// C++ types hidden as void *
typedef void * nanos_wg_t;
typedef void * nanos_team_t;
typedef void * nanos_sched_t;
typedef void * nanos_slicer_t;
typedef void * nanos_dd_t;
typedef void * nanos_sync_cond_t;
typedef unsigned int nanos_copy_id_t;

typedef struct nanos_const_wd_definition_tag {
    nanos_wd_props_t props;
    size_t data_alignment;
    size_t num_copies;
    size_t num_devices;
    size_t num_dimensions;
#ifdef _MF03
   void *description;
#else
   const char *description;
#endif
} nanos_const_wd_definition_t;

typedef struct {
    int nthreads;
    void *arch;
} nanos_constraint_t;

/*!
 * \}
 */ 

#ifdef __cplusplus

#define _Bool bool

extern "C" {
#endif

NANOS_API_DECL(char *, nanos_get_mode, ( void ));

// Functions related to WD
NANOS_API_DECL(nanos_wd_t, nanos_current_wd, (void));
NANOS_API_DECL(int, nanos_get_wd_id, (nanos_wd_t wd));
NANOS_API_DECL(unsigned int, nanos_get_wd_priority, (nanos_wd_t wd));
NANOS_API_DECL(nanos_err_t, nanos_get_wd_description, ( char **description, nanos_wd_t wd ));

// Finder functions
NANOS_API_DECL(nanos_slicer_t, nanos_find_slicer, ( const char * slicer ));
NANOS_API_DECL(nanos_ws_t, nanos_find_worksharing, ( const char * label ) );

NANOS_API_DECL(nanos_err_t, nanos_create_wd_compact, ( nanos_wd_t *wd, nanos_const_wd_definition_t *const_data, nanos_wd_dyn_props_t *dyn_props,
               size_t data_size, void ** data, nanos_wg_t wg, nanos_copy_data_t **copies, nanos_region_dimension_internal_t **dimensions ));

NANOS_API_DECL(nanos_err_t, nanos_set_translate_function, ( nanos_wd_t wd, nanos_translate_args_t translate_args ));

NANOS_API_DECL(nanos_err_t, nanos_create_sliced_wd, ( nanos_wd_t *uwd, size_t num_devices, nanos_device_t *devices,
               size_t outline_data_size, int outline_data_align,
               void **outline_data, nanos_wg_t uwg, nanos_slicer_t slicer,
               nanos_wd_props_t *props, nanos_wd_dyn_props_t *dyn_props, size_t num_copies, nanos_copy_data_t **copies, size_t num_dimensions, nanos_region_dimension_internal_t **dimensions ));

NANOS_API_DECL(nanos_err_t, nanos_submit, ( nanos_wd_t wd, size_t num_data_accesses, nanos_data_access_t *data_accesses, nanos_team_t team ));

NANOS_API_DECL(nanos_err_t, nanos_create_wd_and_run_compact, ( nanos_const_wd_definition_t *const_data, nanos_wd_dyn_props_t *dyn_props,
               size_t data_size, void * data, size_t num_data_accesses, nanos_data_access_t *data_accesses,
               nanos_copy_data_t *copies, nanos_region_dimension_internal_t *dimensions, nanos_translate_args_t translate_args ));

NANOS_API_DECL(nanos_err_t, nanos_create_for, ( void ));

NANOS_API_DECL(nanos_err_t, nanos_set_internal_wd_data, ( nanos_wd_t wd, void *data ));
NANOS_API_DECL(nanos_err_t, nanos_get_internal_wd_data, ( nanos_wd_t wd, void **data ));
NANOS_API_DECL(nanos_err_t, nanos_yield, ( void ));

NANOS_API_DECL(nanos_err_t, nanos_slicer_get_specific_data, ( nanos_slicer_t slicer, void ** data ));

NANOS_API_DECL(nanos_err_t, nanos_get_num_ready_tasks, ( unsigned int *ready_tasks ));
NANOS_API_DECL(nanos_err_t, nanos_get_num_total_tasks, ( unsigned int *total_tasks ));
NANOS_API_DECL(nanos_err_t, nanos_get_num_nonready_tasks, ( unsigned int *nonready_tasks ));
NANOS_API_DECL(nanos_err_t, nanos_get_num_running_tasks, ( unsigned int *running_tasks ));
NANOS_API_DECL(nanos_err_t, nanos_get_num_blocked_tasks, ( unsigned int *blocked_tasks ));

NANOS_API_DECL(nanos_err_t, nanos_in_final, ( bool *result ));
NANOS_API_DECL(nanos_err_t, nanos_set_final, ( bool value ));

// Team related functions

NANOS_API_DECL(nanos_err_t, nanos_create_team,(nanos_team_t *team, nanos_sched_t sg, unsigned int *nthreads,
               nanos_constraint_t * constraints, bool reuse, nanos_thread_t *info));

NANOS_API_DECL(nanos_err_t, nanos_create_team_mapped, (nanos_team_t *team, nanos_sched_t sg, unsigned int *nthreads, unsigned int *mapping));

NANOS_API_DECL(nanos_err_t, nanos_enter_team, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_leave_team, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_end_team, ( nanos_team_t team ));

NANOS_API_DECL(nanos_err_t, nanos_team_barrier, ( void ));

NANOS_API_DECL(nanos_err_t, nanos_single_guard, ( bool *));

NANOS_API_DECL(nanos_err_t, nanos_enter_sync_init, ( bool *b ));
NANOS_API_DECL(nanos_err_t, nanos_wait_sync_init, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_release_sync_init, ( void ));

NANOS_API_DECL(nanos_err_t, nanos_memory_fence, (void));

NANOS_API_DECL(nanos_err_t, nanos_team_get_num_starring_threads, ( int *n ) );
NANOS_API_DECL(nanos_err_t, nanos_team_get_starring_threads, ( int *n, nanos_thread_t *list_of_threads ) );
NANOS_API_DECL(nanos_err_t, nanos_team_get_num_supporting_threads, ( int *n ) );
NANOS_API_DECL(nanos_err_t, nanos_team_get_supporting_threads, ( int *n, nanos_thread_t *list_of_threads) );
NANOS_API_DECL(nanos_err_t, nanos_register_reduction, ( nanos_reduction_t *red) );
NANOS_API_DECL(nanos_err_t, nanos_reduction_get_private_data, ( void **copy, void *original ) );

NANOS_API_DECL(nanos_err_t, nanos_reduction_get, ( nanos_reduction_t **dest, void *original ) );

// dependence
NANOS_API_DECL(nanos_err_t, nanos_dependence_pendant_writes, ( bool *res, void *addr ));

// worksharing
NANOS_API_DECL(nanos_err_t, nanos_worksharing_create ,( nanos_ws_desc_t **wsd, nanos_ws_t ws, nanos_ws_info_t *info, bool *b ) );
NANOS_API_DECL(nanos_err_t, nanos_worksharing_next_item, ( nanos_ws_desc_t *wsd, nanos_ws_item_t *wsi ) );

// sync

NANOS_API_DECL(nanos_err_t, nanos_wg_wait_completion, ( nanos_wg_t wg, bool avoid_flush ));

NANOS_API_DECL(nanos_err_t, nanos_create_int_sync_cond, ( nanos_sync_cond_t *sync_cond, volatile int *p, int condition ));
NANOS_API_DECL(nanos_err_t, nanos_create_bool_sync_cond, ( nanos_sync_cond_t *sync_cond, volatile bool *p, bool condition ));
NANOS_API_DECL(nanos_err_t, nanos_sync_cond_wait, ( nanos_sync_cond_t sync_cond ));
NANOS_API_DECL(nanos_err_t, nanos_sync_cond_signal, ( nanos_sync_cond_t sync_cond ));
NANOS_API_DECL(nanos_err_t, nanos_destroy_sync_cond, ( nanos_sync_cond_t sync_cond ));

NANOS_API_DECL(nanos_err_t, nanos_wait_on, ( size_t num_data_accesses, nanos_data_access_t *data_accesses ));

#define NANOS_INIT_LOCK_FREE { NANOS_LOCK_FREE }
#define NANOS_INIT_LOCK_BUSY { NANOS_LOCK_BUSY }
NANOS_API_DECL(nanos_err_t, nanos_init_lock, ( nanos_lock_t **lock ));
NANOS_API_DECL(nanos_err_t, nanos_init_lock_at, ( nanos_lock_t *lock ));
NANOS_API_DECL(nanos_err_t, nanos_set_lock, (nanos_lock_t *lock));
NANOS_API_DECL(nanos_err_t, nanos_unset_lock, (nanos_lock_t *lock));
NANOS_API_DECL(nanos_err_t, nanos_try_lock, ( nanos_lock_t *lock, bool *result ));
NANOS_API_DECL(nanos_err_t, nanos_destroy_lock, ( nanos_lock_t *lock ));
NANOS_API_DECL(nanos_err_t, nanos_get_lock_address, ( void *addr, nanos_lock_t **lock ));

// Device copies
NANOS_API_DECL(nanos_err_t, nanos_set_copies, (nanos_wd_t wd, int num_copies, nanos_copy_data_t *copies));
NANOS_API_DECL(nanos_err_t, nanos_get_addr, ( nanos_copy_id_t copy_id, void **addr, nanos_wd_t cwd ));

NANOS_API_DECL(nanos_err_t, nanos_copy_value, ( void *dst, nanos_copy_id_t copy_id, nanos_wd_t cwd ));

// system interface
NANOS_API_DECL(const char *, nanos_get_default_architecture, ( void ));
NANOS_API_DECL(const char *, nanos_get_pm, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_get_default_binding, ( bool *res ));

NANOS_API_DECL(nanos_err_t, nanos_delay_start, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_start, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_finish, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_current_socket, ( int socket ));
NANOS_API_DECL(nanos_err_t, nanos_get_num_sockets, ( int *num_sockets ));

// Memory management
NANOS_API_DECL(nanos_err_t, nanos_malloc, ( void **p, size_t size, const char *file, int line ));
NANOS_API_DECL(nanos_err_t, nanos_free, ( void *p ));
NANOS_API_DECL(void, nanos_free0, ( void *p )); 

// error handling
NANOS_API_DECL(void, nanos_handle_error, ( nanos_err_t err ));

// instrumentation interface
NANOS_API_DECL(nanos_err_t, nanos_instrument_register_key, ( nanos_event_key_t *event_key, const char *key, const char *description, bool abort_when_registered ));
NANOS_API_DECL(nanos_err_t, nanos_instrument_register_value, ( nanos_event_value_t *event_value, const char *key, const char *value, const char *description, bool abort_when_registered ));

NANOS_API_DECL(nanos_err_t, nanos_instrument_register_value_with_val, ( nanos_event_value_t val, const char *key, const char *value, const char *description, bool abort_when_registered ));

NANOS_API_DECL(nanos_err_t, nanos_instrument_get_key, (const char *key, nanos_event_key_t *event_key));
NANOS_API_DECL(nanos_err_t, nanos_instrument_get_value, (const char *key, const char *value, nanos_event_value_t *event_value));


NANOS_API_DECL(nanos_err_t, nanos_instrument_events, ( unsigned int num_events, nanos_event_t events[] ));

NANOS_API_DECL(nanos_err_t, nanos_instrument_close_user_fun_event,( void ));

NANOS_API_DECL(nanos_err_t, nanos_instrument_enable,( void ));

NANOS_API_DECL(nanos_err_t, nanos_instrument_disable,( void ));

NANOS_API_DECL(nanos_err_t, nanos_memcpy, (void *dest, const void *src, size_t n));

// scheduling interface
NANOS_API_DECL(const char *, nanos_get_default_scheduler, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_start_scheduler, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_stop_scheduler, ( void ));
NANOS_API_DECL(nanos_err_t, nanos_scheduler_enabled, ( bool *res ));
NANOS_API_DECL(nanos_err_t, nanos_wait_until_threads_paused, ( void ) );
NANOS_API_DECL(nanos_err_t, nanos_wait_until_threads_unpaused, ( void ) );
NANOS_API_DECL(nanos_err_t, nanos_scheduler_get_stealing, ( bool *res ));
NANOS_API_DECL(nanos_err_t, nanos_scheduler_set_stealing, ( bool value ));


// utility macros

#define NANOS_SAFE( call ) \
do {\
   nanos_err_t err = call;\
   if ( err != NANOS_OK ) nanos_handle_error( err );\
} while (0)

void nanos_reduction_int_vop ( int, void *, void * );

#ifdef __cplusplus
}
#endif

#endif
