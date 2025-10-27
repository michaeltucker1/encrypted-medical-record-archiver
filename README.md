# Medical Record Archiver

A simple C90 command line program that stores encrypted medical records with compression and encryption. 

## Features
- **Automatic Encryption**: XOR-based encryption behind the scenes
- **Compression**: Run Length Encoding (RLE) to save space
- **Simple Commands**: `--add`, `--view`, `--search`, `--delete`, `--sort`

## Building the Program

Compile the program using the included Makefile:

```bash
make clean
make
```

## Usage
```bash
./medical_archiver <mode>
```

### Commands
#### Add a patient record
```bash
./medical_archiver --add
```
Prompts you to enter patient record data. The program automatically:
- Creates the data file if it doesn't exist
- Uses default encryption password
- Compresses and encrypts the data
- Saves to `medical.dat`

**Record Format:**
```
name:John Doe;age:45;diagnosis:Flu;notes:Recovered
```

#### View all records
```bash
./medical_archiver --view
```
Displays all patient records in the archive. Automatically:
- Loads from `medical.dat`
- Decrypts and decompresses data
- Shows formatted patient information

#### Search records
```bash
./medical_archiver --search <term>
```
Searches for records containing the specified term.

**Examples:**
```bash
./medical_archiver --search diabetes
./medical_archiver --search "John Doe"
./medical_archiver --search 25
```

#### Delete records
```bash
./medical_archiver --delete <id>
```
Deletes records by ID number.

**Example:**
```bash
# Delete by ID
./medical_archiver --delete 1
```

#### Show help
```bash
./medical_archiver --help
```
Displays usage information.

## Complete Workflow

```bash
./medical_archiver --add
# Enter: name:Alice Smith;age:25;diagnosis:Common Cold;notes:Rest recommended

# Add another record
./medical_archiver --add
# Enter: name:Bob Johnson;age:45;diagnosis:Diabetes;notes:Monitor glucose

# View all records
./medical_archiver --view

# Search for specific conditions
./medical_archiver --search diabetes

# Delete a specific record by ID
./medical_archiver --delete 2

# Sort remaining records alphabetically
./medical_archiver --sort

# Get help anytime
./medical_archiver --help
```