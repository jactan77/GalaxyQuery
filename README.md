# GalaxyQuery

GalaxyQuery is a lightweight, SQL-inspired, in-memory database system written in modern C++ designed to handle table and column manipulation. It provides basic database functionalities such as table creation, column management, data insertion, updates, conditions-based filtering, and query interpretation. This project is ideal for educational purposes, lightweight systems, or as a proof-of-concept for understanding database systems.

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

## Requirements

* **C++ Version**: C++20 or later.
* **Build System**: CMake.
* **Dependencies**: Standard C++ library (no external dependencies).

## Usage

### Setup

1. Clone the repository:

```
git clone <repository_url>
cd GalaxyQuery
```

2. Build the project with CMake:

```
mkdir build
cd build
cmake ..
make
```

### Sample Usage

Below are a few examples of how to use the system via the query interpreter:

1. Create a Table

```
CREATE TABLE students (id INT, name STRING, grade INT);
```

2. Insert Values

```
INSERT INTO students (id, name, grade) VALUES (1, "Alice", 95);
INSERT INTO students (id, name, grade) VALUES (2, "Bob", 88);
```

3. Query Data

```
SELECT name, grade FROM students WHERE grade > 90;
```

4. Update Values

```
UPDATE students SET grade = 92 WHERE name = "Bob";
```

5. Delete Values

```
DELETE FROM students WHERE grade < 90;
```

6. Drop a Column

```
ALTER TABLE students DROP COLUMN grade;
```

## Code Structure

### Core Components

1. **Table.cpp**:
    * Handles table-level functionality, like column management, data updates, and query processing.
2. **Column.cpp**:
    * Represents individual columns, providing utilities for value insertion, type checking, and filtering operations.
3. **InterpreterQuery.cpp**:
    * Implements the logic for SQL-like query parsing and tokenizing commands for operations like `CREATE`, `INSERT`, `SELECT`, `DELETE`, `UPDATE`.


## License

This project is open-source and licensed under the MIT License.
