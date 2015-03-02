/*-------------------------------------------------------------------------
 *
 * tuple_schema.h
 * file description
 *
 * Copyright(c) 2015, CMU
 *
 * /n-store/src/catalog/tuple_schema.h
 *
 *-------------------------------------------------------------------------
 */

#pragma once

#include <vector>

#include "common/types.h"

namespace nstore {
namespace catalog {

//===--------------------------------------------------------------------===//
// Column Information
//===--------------------------------------------------------------------===//

class ColumnInfo {
	ColumnInfo() = delete;
	ColumnInfo &operator=(const ColumnInfo &) = delete;

public:

	// Configures all members except offset
	ColumnInfo(ValueType column_type, uint32_t column_length, bool allow_null,
			bool is_inlined)
: type(column_type), offset(0), allow_null(allow_null), is_inlined(is_inlined){
		if(is_inlined){
			fixed_length = column_length;
			variable_length = 0;
		}
		else{
			fixed_length = sizeof(uintptr_t);
			variable_length = column_length;
		}
	}

	/// Compare two column info objects
	bool operator== (const ColumnInfo &other) const {
		if (other.allow_null != allow_null || other.type != type ||
				other.is_inlined != is_inlined) {
			return false;
		}

		return true;
	}

	bool operator!= (const ColumnInfo &other) const {
		return !(*this == other);
	}

	// Configure offset as well
	ColumnInfo(ValueType column_type, uint32_t column_offset, uint32_t column_length,
			bool allow_null, bool is_inlined)
: type(column_type), offset(column_offset), allow_null(allow_null),
  is_inlined(is_inlined){

		if(is_inlined){
			fixed_length = column_length;
			variable_length = 0;
		}
		else{
			fixed_length = sizeof(uintptr_t);
			variable_length = column_length;
		}
	}

	/// Get a string representation for debugging
	friend std::ostream& operator<< (std::ostream& os, const ColumnInfo& column_info);

	//===--------------------------------------------------------------------===//
	// Data members
	//===--------------------------------------------------------------------===//

	/// type of column
	ValueType type;

	/// offset of column in tuple
	uint32_t offset;

	/// if the column is not inlined, this is set to pointer size
	/// else, it is set to length of the fixed length column
	uint32_t fixed_length;

	/// if the column is inlined, this is set to 0
	/// else, it is set to length of the variable length column
	uint32_t variable_length;

	bool allow_null;
	bool is_inlined;
};

//===--------------------------------------------------------------------===//
// Schema
//===--------------------------------------------------------------------===//

class Schema	{
	friend class ColumnInfo;

public:
	Schema() = delete;

	//===--------------------------------------------------------------------===//
	// Static factory methods to construct schema objects
	//===--------------------------------------------------------------------===//

	/// Construct schema
	void CreateTupleSchema(const std::vector<ValueType> column_types,
			const std::vector<uint32_t> column_lengths,
			const std::vector<bool> allow_null,
			const std::vector<bool> is_inlined);

	/// Construct schema from vector of ColumnInfo
	Schema(const std::vector<ColumnInfo> columns);

	/// Copy schema
	static Schema *CopySchema(const Schema *schema);

	/// Copy subset of columns in the given schema
	static Schema *CopySchema(const Schema *schema, const std::vector<uint32_t>& set);

	/// Append two schema objects
	static Schema *AppendSchema(const Schema *first, const Schema *second);

	/// Append subset of columns in the two given schemas
	static Schema *AppendSchema(const Schema *first, const std::vector<uint32_t>& first_set,
			const Schema *second, const std::vector<uint32_t>& second_set);

	/// Compare two schemas
	bool operator== (const Schema &other) const;
	bool operator!= (const Schema &other) const;

	//===--------------------------------------------------------------------===//
	// Schema accessors
	//===--------------------------------------------------------------------===//

	inline uint32_t GetOffset(const uint32_t column_id) const {
		return columns[column_id].offset;
	}

	inline ValueType GetType(const uint32_t column_id) const {
		return columns[column_id].type;
	}

	/// Returns fixed length
	inline uint32_t GetLength(const uint32_t column_id) const {
		return columns[column_id].fixed_length;
	}

	inline uint32_t GetVariableLength(const uint32_t column_id) const {
		return columns[column_id].variable_length;
	}

	//// Get the nullability of the column at a given index.
	inline bool AllowNull(const uint32_t column_id) const {
		return columns[column_id].allow_null;
	}


	inline bool IsInlined(const uint32_t column_id) const {
		return columns[column_id].is_inlined;
	}

	const ColumnInfo GetColumnInfo(const uint32_t column_id) const {
		return columns[column_id];
	}

	int GetUninlinedColumnIndex(const int column_id) const {
		return uninlined_columns[column_id];
	}

	/// Return the number of columns in the schema for the tuple.
	inline int GetColumnCount() const {
		return column_count;
	}

	int GetUninlinedColumnCount() const {
		return uninlined_column_count;
	}

	/// Return the number of bytes used by one tuple.
	inline uint32_t GetLength() const {
		return length;
	}

	/// Returns a flag indicating whether all columns are inlined
	bool IsInlined() const {
		return is_inlined;
	}

	/// Get a string representation of this schema
	friend std::ostream& operator<<(std::ostream& os, const Schema& schema);

private:
	// size of fixed length columns
	uint32_t length;

	// all inlined and uninlined columns in the tuple
	std::vector<ColumnInfo> columns;

	// keeps track of unlined columns
	std::vector<int> uninlined_columns;

	// keep these in sync with the vectors above
	int column_count;

	int uninlined_column_count;

	/// are all columns inlined
	bool is_inlined;

};

} // End catalog namespace
} // End nstore namespace