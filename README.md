# GalaxyQuery

GalaxyQuery is a lightweight, SQL-inspired, in-memory database system written in modern C++ designed to handle table and column manipulation with persistent data storage capabilities. It provides basic database functionalities such as table creation, column management, data insertion, updates, conditions-based filtering, query interpretation, and data persistence to file system. This project is ideal for educational purposes, lightweight systems, or as a proof-of-concept for understanding database systems.

## Features

* **Table Operations**:
  * Create, drop, and alter tables.
  * Add, delete, or rename columns.
  * Print tables in a formatted structure.
  * Clear table rows while preserving table schemas.
* **Column Management**:
  * Validate data types dynamically (`INT`, `STRING`, `BOOL`, etc.).
  * Insert, update, and query column data.
  * Print column data with calculated widths for easy reading.
  * Default values insertion for columns when data is missing.
* **Query Interpreter**:
  * SQL-like commands (`SELECT`, `INSERT`, `UPDATE`, `DELETE`, etc.).
  * Tokenizer to parse and process input queries.
  * Condition parsing for filtering rows with operators (`=`, `>`, `<`, `!=`).
* **Dynamic Typing**:
  * Automatic type detection based on input values.
  * String analysis to identify valid data types during insertion.
* **Data Persistence**:
  * Save database state to file system (`data.txt`).
  * Load database state from file on startup.
  * Automatic data serialization and deserialization.
  * Custom file format for efficient storage.

## Requirements

* **C++ Version**: C++20 or later.
* **Build System**: CMake.
* **Dependencies**: Standard C++ library (no external dependencies).

## Usage

### Setup

1. Clone the repository:

```bash
git clone <repository_url>
cd GalaxyQuery
```

2. Build the project with CMake:

```bash
mkdir build
cd build
cmake ..
make
```

### Sample Usage

Below are a few examples of how to use the system via the query interpreter:

1. **Create a Table**

```sql
CREATE TABLE students (id INT, name STRING, grade INT);
```

2. **Insert Values**

```sql
INSERT INTO students (id, name, grade) VALUES (1, Alice, 95);
INSERT INTO students (id, name, grade) VALUES (2, Bob, 88);
```

3. **Query Data**

```sql
SELECT (name, grade) FROM students WHERE (grade > 90)
```

4. **Update Values**

```sql
UPDATE students SET (grade = 92) WHERE (name = Bob)
```

5. **Delete Values**

```sql
DELETE FROM students;
```

6. **Drop a Column**

```sql
ALTER TABLE students DROP COLUMN grade;
```

### Data Persistence

The database automatically saves data to `data.txt` file and loads it on startup. The file format is:

```
DB:<database_name>
<table_name>:<column1_name>(<data_type>){value1,value2,...}:<column2_name>(<data_type>){value1,value2,...}
```

Example:
```
DB:MyDatabase
students:id(INT){1,2}:name(STRING){Alice,Bob}:grade(INT){95,88}
```

## Code Structure

### Core Components

1. **Table.cpp**:
  * Handles table-level functionality, like column management, data updates, and query processing.
2. **Column.cpp**:
  * Represents individual columns, providing utilities for value insertion, type checking, and filtering operations.
3. **InterpreterQuery.cpp**:
  * Implements the logic for SQL-like query parsing and tokenizing commands for operations like `CREATE`, `INSERT`, `SELECT`, `DELETE`, `UPDATE`.
4. **GalaxyQueryLoader.cpp**:
  * Handles loading database state from file system.
  * Parses the custom file format to reconstruct database objects.
  * Uses regex patterns for efficient data extraction.
5. **GalaxyQueryExporter.cpp**:
  * Handles saving database state to file system.
  * Serializes database objects to custom file format.
  * Ensures data integrity during export operations.

### File Format Specification

The persistence layer uses a custom text-based format:

* **Database Header**: `DB:<database_name>`
* **Table Format**: `<table_name>:<column_definitions>`
* **Column Format**: `<column_name>(<data_type>){<comma_separated_values>}`
* **Multiple Columns**: Separated by `:` character

### Key Implementation Details

* Uses C++20 ranges and views for efficient string processing
* Regex-based parsing for robust data extraction
* Memory management with proper cleanup
* Automatic ID generation for table rows
* File size validation for empty database handling

## Error Handling

* Validates file existence and size before loading
* Handles empty databases gracefully
* Robust regex parsing with fallback mechanisms
* Memory safety with proper pointer management

## Performance Considerations

* In-memory operations for fast query execution
* Efficient string processing using ranges
* Minimal file I/O overhead
* Lazy loading of database state

## License

This project is open-source and licensed under the MIT License.