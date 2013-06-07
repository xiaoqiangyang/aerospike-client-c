/******************************************************************************
 * Copyright 2008-2013 by Aerospike.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

#pragma once 

#include <aerospike/aerospike.h>
#include <aerospike/as_binop.h>
#include <aerospike/as_error.h>
#include <aerospike/as_list.h>
#include <aerospike/as_policy.h>
#include <aerospike/as_record.h>
#include <aerospike/as_status.h>
#include <aerospike/as_val.h>

/******************************************************************************
 * FUNCTIONS
 *****************************************************************************/

/**
 * Look up a record by key, then return all bins.
 *
 *      as_record * rec = NULL;
 *      if ( aerospike_key_get(&as, &err, NULL, "test", "demo", "foo", &rec) != AEROSPIKE_OK ) {
 *          fprintf(stderr, "error(%d) %s at [%s:%d]", err.code, err.message, err.file, err.line);
 *      }
 *
 * @param as        - the aerospike cluster to connect to
 * @param err       - the error is populated if the return value is not AEROSPIKE_OK.
 * @param policy    - the policy to use for this operation. If NULL, then the default policy will be used.
 * @param ns        - the namespace of the record.
 * @param set       - the set-name of the record.p
 * @param key       - the key of the record. Can be either as_integer or as_string.
 * @param rec       - the record to be populated with the data from request.
 *
 * @return AEROSPIKE_OK if successful. Otherwise an error.
 */
as_status aerospike_key_get(
	aerospike * as, as_error * err, const as_policy_read * policy, 
	const char * ns, const char * set, const char * key, 
	as_record ** rec
	);

/**
 * Lookup a record by key, then return specified bins.
 *
 *      as_record * rec = NULL;
 *      char * select[] = {"bin1", "bin2", "bin3", NULL};
 *      if ( aerospike_key_select(&as, &err, NULL, "test", "demo", "foo", select, &rec) != AEROSPIKE_OK ) {
 *          fprintf(stderr, "error(%d) %s at [%s:%d]", err.code, err.message, err.file, err.line);
 *      }
 *
 * @param as        - the aerospike cluster to connect to
 * @param err       - the error is populated if the return value is not AEROSPIKE_OK.
 * @param policy    - the policy to use for this operation. If NULL, then the default policy will be used.
 * @param ns        - the namespace of the record.
 * @param set       - the set of the record.
 * @param key       - the key of the record. Can be either as_integer or as_string.
 * @param bins      - the bins to select. A NULL terminated array of NULL terminated strings.
 *
 * @return AEROSPIKE_OK if successful. Otherwise an error.
 */
as_status aerospike_key_select(
	aerospike * as, as_error * err, const as_policy_read * policy, 
	const char * ns, const char * set, const char * key, 
	const char * bins[], 
	as_record ** rec
	);

/**
 * Check if a record exists in the cluster via its key.
 *
 *      bool exists = true;
 *      if ( aerospike_key_exists(&as, &err, NULL, "test", "demo", "foo", &exists) != AEROSPIKE_OK ) {
 *          fprintf(stderr, "error(%d) %s at [%s:%d]", err.code, err.message, err.file, err.line);
 *      }
 *      else {
 *          fprintf(stdout, "Record %s", exists ? "exists." : "doesn't exist.");
 *      }
 *
 * @param as        - the aerospike cluster to connect to
 * @param err       - the error is populated if the return value is not AEROSPIKE_OK.
 * @param policy    - the policy to use for this operation. If NULL, then the default policy will be used.
 * @param ns        - the namespace of the record.
 * @param set       - the set of the record.
 * @param key       - the key of the record. Can be either as_integer or as_string.
 * @param exists    - will be `true` if the record exists, otherwise `false`.
 *
 * @return AEROSPIKE_OK if successful. Otherwise an error occurred. Check the `err` for details on the error.
 */
as_status aerospike_key_exists(
	aerospike * as, as_error * err, const as_policy_read * policy, 
	const char * ns, const char * set, const char * key,
	bool * exists
	);

/**
 * Store a record in the cluster.
 *
 *      as_record * rec = as_record_new(2);
 *      as_record_set_string(rec, "bin1", "abc");
 *      as_record_set_integer(rec, "bin2", 123);
 *
 *      if ( aerospike_key_put(&as, &err, NULL, "test", "demo", "foo", rec) != AEROSPIKE_OK ) {
 *          fprintf(stderr, "error(%d) %s at [%s:%d]", err.code, err.message, err.file, err.line);
 *      }
 *
 * @param as        - the aerospike cluster to connect to
 * @param err       - the error is populated if the return value is not AEROSPIKE_OK.
 * @param policy    - the policy to use for this operation. If NULL, then the default policy will be used.
 * @param ns        - the namespace of the record.
 * @param set       - the set of the record.
 * @param key       - the key of the record. Can be either as_integer or as_string.
 *
 * @return AEROSPIKE_OK if successful. Otherwise an error occurred. Check the `err` for details on the error.
 */
as_status aerospike_key_put(
	aerospike * as, as_error * err, const as_policy_write * policy, 
	const char * ns, const char * set, const char * key, 
	as_record * rec
	);

/**
 * Remove a record from the cluster.
 *
 *      if ( aerospike_key_remove(&as, &err, NULL, "test", "demo", "foo") != AEROSPIKE_OK ) {
 *          fprintf(stderr, "error(%d) %s at [%s:%d]", err.code, err.message, err.file, err.line);
 *      }
 *
 * @param as        - the aerospike cluster to connect to
 * @param err       - the error is populated if the return value is not AEROSPIKE_OK.
 * @param policy    - the policy to use for this operation. If NULL, then the default policy will be used.
 * @param ns        - the namespace of the record.
 * @param set       - the set of the record.
 * @param key       - the key of the record. Can be either as_integer or as_string.
 *
 * @return AEROSPIKE_OK if successful. Otherwise an error occurred. Check the `err` for details on the error.
 */
as_status aerospike_key_remove(
	aerospike * as, as_error * err, const as_policy_remove * policy, 
	const char * ns, const char * set, const char * key
	);

/**
 * Lookup a record by key, then apply the UDF.
 *
 *      as_list args;
 *      as_arraylist_init(&args, 2, 0);
 *      as_list_add_integer(&args, 1);
 *      as_list_add_integer(&args, 2);
 *
 *      as_val * res = NULL;
 *
 *      if ( aerospike_key_apply(&as, &err, NULL, "test", "demo", "foo", "math", "add", &args, &res) != AEROSPIKE_OK ) {
 *          fprintf(stderr, "error(%d) %s at [%s:%d]", err.code, err.message, err.file, err.line);
 *      }
 *
 * @param as        - the aerospike cluster to connect to
 * @param err       - the error is populated if the return value is not AEROSPIKE_OK.
 * @param policy    - the policy to use for this operation. If NULL, then the default policy will be used.
 * @param ns        - the namespace of the record.
 * @param set       - the set of the record.
 * @param key       - the key of the record. Can be either as_integer or as_string.
 * @param module    - the module containing the function to execute
 * @param function  - the function to execute
 * @param arglist   - arguments for the function
 * @param result    - the return value from the function
 *
 * @return AEROSPIKE_OK if successful. Otherwise an error occurred. Check the `err` for details on the error.
 */
as_status aerospike_key_apply(
	aerospike * as, as_error * err, const as_policy_read * policy, 
	const char * ns, const char * set, const char * key, 
	const char * module, const char * function, as_list * arglist, 
	as_val ** result
	);

/**
 * Lookup a record by key, then perform specified operations.
 * 
 * @param as        - the aerospike cluster to connect to.
 * @param err       - the error is populated if the return value is not AEROSPIKE_OK.
 * @param policy    - the policy to use for this operation. If NULL, then the default policy will be used.
 * @param ns        - the namespace of the record.
 * @param set       - the set of the record.
 * @param key       - the key of the record. Can be either as_integer or as_string.
 * @param ops       - an array of as_bin_operation, which specify the operation to perform on bins of the record.
 * @param nops      - the number of operations.
 *
 * @return AEROSPIKE_OK if successful. Otherwise an error occurred. Check the `err` for details on the error.
 */
as_status aerospike_key_operate(
	aerospike * as, as_error * err, const as_policy_write * policy, 
	const char * ns, const char * set, const char * key, 
	as_binop * ops, uint32_t nops
	);