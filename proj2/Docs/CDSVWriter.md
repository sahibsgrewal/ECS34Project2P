# DSVWriter Documentation

## Overview

The **DSVWriter** class is responsible for writing delimiter-separated values (DSV) data to a data sink. It supports configurable delimiters and quoting options to handle special characters within field values.

## Class: `CDSVWriter`

The `CDSVWriter` class provides an interface for writing structured data to a sink in a delimiter-separated format.

### Constructor

```cpp
CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall);
```

- **Parameters:**
  - `sink`:  A shared pointer to a `CDataSink` object where the DSV data will be written.
  - `delimiter`: The character used to separate fields in a row.
  - `quoteall`: A boolean indicating whether all fields should be enclosed in quotes.

- **Description:**
  - Initializes a `CDSVWriter` instance with the specified sink, delimiter, and quoting preference.

#### Destructor

```cpp
~CXMLWriter();
```

- **Description:**
  - Cleans up resources used by `CDSVWriter`.

#### Methods

##### `bool WriteRow(const std::vector<std::string>& dataRow);`

- **Parameters:**
  - `dataRow`: A vector of strings representing the fields in a row.

- **Returns:**
  - `true` if the row was successfully written, otherwise `false`.

- **Description:**
  - Writes a row of data to the sink, applying quoting rules if necessary.


### SImplementation Struct

The `SImplementation` struct manages the internal logic of the DSV writing process.

#### Constructor

```cpp
SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall);
```

- **Parameters:**
  - `sink`: A shared pointer to a data sink for writing output.
  - `delimiter`: The character used to separate fields.
  - `quoteall`: A boolean indicating whether all fields should be enclosed in quotes.

- **Description:**
  - Initializes an `SImplementation` object with the given sink, delimiter, and quoting preference.

```cpp
SImplementation();;
```
- **Description:**
  - Default constructor initializing with no sink, a comma as the delimiter, and `quoteall` set to `false`.


#### Methods

##### `bool WriteRow(const std::vector<std::string>& dataRow);`

- **Description:**
  - Processes a row of data, applying quoting rules if needed, and writes it to the data sink.