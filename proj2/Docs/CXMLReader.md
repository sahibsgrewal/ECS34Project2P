Class: CXMLReader
This class provides an interface for reading XML data from a data source.

Constructor:

CXMLReader(std::shared_ptr<CDataSource> source): Initializes an instance of CXMLReader, creating a unique instance of SImplementation with the provided data source.
Destructor:

~CXMLReader(): Default destructor for CXMLReader.
Public Methods:

bool End() const: Checks if the end of the XML stream has been reached by delegating the call to the IsEnd() method of the SImplementation.
bool ReadEntity(SXMLEntity& entity, bool skipCData): Reads the next XML entity from the data source through the ReadEntity method of SImplementation.
Usage
To use the XML reader implementation:

Create a data source that inherits from CDataSource.
Instantiate CXMLReader with the data source.
Use the ReadEntity method to read and process XML entities from the data source.
Check for the end of the XML stream using the End method.