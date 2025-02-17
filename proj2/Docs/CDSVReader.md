# DSVReader Documentation

## Overview

The **DSVReader** library is designed to read delimiter-separated value (DSV) files, supporting configurable delimiters and quoted values. 

## Class: `CDSVReader`

The `CDSVReader` class provides an interface to read rows of data from a data source.

### Constructor

```cpp
CDSVReader(std::shared_ptr<CDataSource> dc, char delimiter);
```

- **Parameters:**
  - `dc`: A shared pointer to a `CDataSource` object, which provides the data stream
  - `delimiter`: The character that's used to seperate values within each row

- **Description:**
  - Initializes a `CDSVReader` object with the given `CDataSource` and delimiter.

#### Destructor

```cpp
~CDSVReader();
```

- **Description:**
  - Cleans up reasources used by `CDSVReader`.

#### Methods

##### `bool End() const;`

- **Returns:**
  - `true` if the end of the data source is reached, otherwise `false`.

- **Description:**
  - Checks if the end of the data source has been reached.

##### `bool ReadRow(std::vector<std::string>& row);`

- **Parameters:**
  - `row`: A reference to a vector of strings where the read fields will be stored.

- **Returns:**
  - `true` if a row is successfully read, otherwise `false`.

- **Description:**
  - Reads a row from the data source and populates the provided vector with the fields.

### SImplementation Struct

The `SImplementation` struct handles internal mechanics of reading and parsing rows.

#### Constructor

```cpp
SImplementation(std::shared_ptr<CDataSource> dc, char delimiter);
```

- **Parameters:**
  - `dc`: A shared pointer to a data source.
  - `delimiter`: The character used to separate fields in each row.

- **Description:**
  - Initializes an `SImplementation` object with the given `CDataSource` and delimiter.

#### Methods

##### `bool End() const;`

- **Returns:**
  - `true` if the end of the data source is reached, otherwise `false`.

- **Description:**
  - Checks if the end of the data source has been reached.

##### `bool ReadRow(std::vector<std::string>& row);`

- **Parameters:**
  - `row`: A reference to a vector of strings where the read fields will be stored.

- **Returns:**
  - `true` if a row is successfully read, otherwise `false` (indicating the end of the data source).

- **Description:**
  - Reads a row from the data source and populates the provided vector with the fields.

### Reading Rows

The `ReadRow` function is used to read a row from the data source. The function reads characters until it encounters the specified 
delimiter or a newline character, handling cases where the delimiter is enclosed within double quotes. The read fields are stored 
in the provided vector.

### End-of-Data Check

The `End` function is provided to check whether the end of the data source has been reached. It returns `true` if the end is reached 
and `false` otherwise.