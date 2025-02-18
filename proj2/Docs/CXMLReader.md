# XMLReader Documentation

## Overview

The **XMLReader** library provides functionality for parsing XML data from a data source. It utilizes the Expat library for XML parsing and processes various XML entities while handling character data, start elements, and end elements. 

## Class: `CXMLReader`

The `CXMLReader` class serves as the primary interface for reading XML data.

### Constructor

```cpp
CXMLReader(std::shared_ptr<CDataSource> source);
```

- **Parameters:**
  - `source`: A shared pointer to a `CDataSource` object, which provides the XML input.

- **Description:**
  - Initializes an XML reader using the specified data source.

#### Destructor

```cpp
~CXMLReader();
```

- **Description:**
  - Cleans up reasources used by `CXMLReader`.

#### Methods

##### `bool End() const;`

- **Returns:**
  - `true` if the end of the data source is reached and the queue is empty, otherwise `false`.

- **Description:**
  - Checks whether the XML reader has reached the end of the input data.

##### `bool ReadEntity(SXMLEntity& entity, bool skipCData);`

- **Parameters:**
  - `entity`: A reference to an SXMLEntity object where the read XML entity will be stored.
  - `skipCData`: A boolean indicating whether to skip character data (true to skip, false to include).

- **Returns:**
  - `true` if an entity was successfully read, otherwise `false`.

- **Description:**
  - Reads an XML entity (element, character data, etc.) from the data source.

### SImplementation Struct

The `SImplementation` struct manages the internal XML parsing logic.

#### Constructor

```cpp
SImplementation(std::shared_ptr<CDataSource> source);
```

- **Parameters:**
  - `source`: Shared pointer to a data source.

- **Description:**
  - Initializes an `SImplementation` object with the specified data source and sets up the Expat XML parser.

#### Methods

##### `bool IsEnd() const;`

- **Returns:**
  - `true` if the data source has reached the end and the entity queue is empty, otherwise `false`.

- **Description:**
  - Checks whether the XML reader has fully processed all available input.


##### `bool ReadEntity(SXMLEntity& entity, bool skipCData);`

- **Parameters:**
  - `entity`: A reference to an `SXMLEntity` object where the read XML entity will be stored.
  - `skipCData`: A boolean indicating whether to skip character data (true to skip, false to include).

- **Returns:**
  - `true` if an entity was successfully read, otherwise `false`

- **Description:**
  - Reads an entity from the queue, optionally skipping character data


##### `void RefillEntityQueue();`

- **Description:**
  - Fills the entity queue by reading from the data source and parsing XML data.


##### `void HandleStartElement(const std::string& name, const std::vector<std::string>& attributes);`

- **Parameters:**
  - `name`: The name of the XML element.
  - `attributes`: A vector of attribute name-value pairs.

- **Description:**
  - Processes a start element and adds it to the entity queue


##### `void HandleEndElement(const std::string& name);`

- **Parameters:**
  - `name`: The name of the XML element.

- **Description:**
  - Processes an end element and adds it to the entity queue. 


##### `void HandleCharacterData(const std::string& data);`

- **Parameters:**
  - `data`: The character data content.

- **Description:**
  - Processes character data and adds it to the entity queue, merging consecutive text nodes.


##### `static void StartElementHandler(void* context, const XML_Char* name, const XML_Char** attributes);`

- **Parameters:**
  - `context`: A pointer to the `SImplementation` instance.
  - `name`: The name of the XML element.
  - `attributes`: A vector of attribute name-value pairs.

- **Description:**
  - Callback function for handling start elements in the XML parser


##### `static void EndElementHandler(void* context, const XML_Char* name);`

- **Parameters:**
  - `context`: A pointer to the `SImplementation` instance.
  - `name`: The name of the XML element.

- **Description:**
  - Callback function for handling end elements in the XML parser


##### `static void CharacterDataHandler(void* context, const XML_Char* data, int length);`

- **Parameters:**
  - `context`: A pointer to the `SImplementation` instance.
  - `data`: A pointer to the character data.
  - `length`: The length of the character data.

- **Description:**
  - Callback function for handling character data in the XML parser.