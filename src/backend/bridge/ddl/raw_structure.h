//===----------------------------------------------------------------------===//
//
//							PelotonDB
//
// raw_structure.h
//
// Identification: src/backend/bridge/ddl/raw_structure.h
//
// Copyright (c) 2015, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include "backend/common/types.h"

namespace peloton {
namespace bridge {

//===--------------------------------------------------------------------===//
// Raw data structures
//===--------------------------------------------------------------------===//

struct raw_constraint_info {
  ConstraintType constraint_type;
  char *constraint_name;
  Node *expr;
};

struct raw_column_info {
  ValueType column_type;
  oid_t column_length;
  ;
  char *column_name;
  bool is_inlined;
  raw_constraint_info **raw_constraints;
  oid_t constraint_count;
};

struct raw_table_info {
  oid_t table_oid;
  char *table_name;
  // std::vector<raw_column_info*> *raw_columns;
  raw_column_info **raw_columns;
  oid_t column_count;
};

struct raw_index_info {
  char *index_name;
  oid_t index_oid;
  char *table_name;
  IndexType method_type;
  IndexConstraintType constraint_type;
  bool unique_keys;
  char **key_column_names;
  oid_t key_column_count;
};

struct raw_foreignkey_info {
  oid_t source_table_id; // a table that has a reference key
  oid_t sink_table_id;   // a table that has a primary key
  int *source_column_offsets;
  oid_t source_column_count;
  int *sink_column_offsets;
  oid_t sink_column_count;
  char update_action;
  char delete_action;
  char *fk_name;
};

struct raw_database_info {
  oid_t database_oid;
  char *database_name;

  raw_table_info **raw_tables;
  raw_index_info **raw_indexes;
  raw_foreignkey_info **raw_foreignkeys;

  oid_t table_count;
  oid_t index_count;
  oid_t foreignkey_count;
};

} // namespace bridge
} // namespace peloton