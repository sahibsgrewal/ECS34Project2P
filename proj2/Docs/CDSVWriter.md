CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall): Constructs a CDSVWriter instance with the specified parameters for the data sink, delimiter, and quoting behavior.

~CDSVWriter(): Destructor that cleans up the resources associated with the CDSVWriter instance.

bool WriteRow(const std::vectorstd::string& dataRow):

Parameters:
dataRow: A vector of strings representing a single row of data to be written to the CSV.
Returns: A boolean indicating the success of the write operation.
Description: This method processes the provided data row, applying the appropriate formatting rules:
Each field is checked to determine if it requires quoting based on the presence of the delimiter, double quotes, or newline characters.
Fields that require quoting are enclosed in double quotes. Any existing double quotes within the field are escaped by replacing them with two double quotes.
The formatted fields are then concatenated into a single string, with the specified delimiter separating the fields.
Finally, a newline character is appended to indicate the end of the row.
The resulting formatted string is written to the specified data sink.