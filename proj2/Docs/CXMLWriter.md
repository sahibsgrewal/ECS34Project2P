# XMLWriter Documentation

## Overview

The **XMLWriter** library provides functionality for writing XML data to a specified data sink. It handles special character encoding and different types of XML entities, ensuring well-formed XML output.

## Class: `CXMLWriter`

The `CXMLWriter` class is the main class for writing XML content.

### Constructor

```cpp
CXMLWriter(std::shared_ptr<CDataSink> sink)
```

- **Parameters:**
  - `sink`: Shared pointer to a data sink that will receive the XML output

- **Description:**
  - Creates a new XML writer that writes to the specified data sink

#### Destructor

```cpp
~CXMLWriter();
```

- **Description:**
  - Cleans up resources used by `CXMLWriter`.

#### Methods

##### `bool Flush();`

- **Returns:**
  - `true`, as no internal buffering is used.

- **Description:**
  - Ensures that all data has been written.

##### `bool WriteEntity(const SXMLEntity &entity);`

- **Parameters:**
  - `entity`: An instance of SXMLEntity, representing an XML element or character data.

- **Returns:**
  - `true` if the entity was successfully written, otherwise `false`.

- **Description:**
  - Writes an XML entity (element, character data, etc.) to the data sink.

### SImplementation Struct

The `SImplementation` struct manages the internal XML writing logic.

#### Constructor

```cpp
SImplementation(std::shared_ptr<CDataSink> sink);
```

- **Parameters:**
  - `sink`: Shared pointer to a data sink.

- **Description:**
  - Initializes an `SImplementation` object with the specified data sink.


#### Methods

##### `void HandleSpecial(std::ostream& os, const std::string& value);`

- **Parameters:**
  - `os`: Output stream where the encoded data is written.
  - `value`: The string containing special characters to be encoded.

- **Description:**
  - Encodes special XML characters such as &, <, >, ", and ' to their corresponding XML entities.

##### `void StartElement(const std::string& name, const std::vector<SXMLEntity::TAttribute>& attributes);`

- **Parameters:**
  - `name`: Name of the XML element.
  - `attributes`: A vector of key-value pairs representing XML attributes.

- **Description:**
  - Writes a start element tag with optional attributes.

##### `void EndElement(const std::string& name);`

- **Parameters:**
  - `name`: Name of the XML element.

- **Description:**
  - Writes an end element tag.

##### `void CompleteElement(const std::string& name, const std::vector<SXMLEntity::TAttribute>& attributes);`

- **Parameters:**
  - `name`: Name of the XML element.
  - `attributes`: A vector of key-value pairs representing XML attributes.

- **Description:**
  - Writes a self-closing XML element.

##### `void CharData(const std::string& data);`

- **Parameters:**
  - `data`: String containing character data.

- **Description:**
  - Writes character data while encoding special XML characters.