# C++ Machine Learning Pipeline
A modular C++ machine learning pipeline for data extraction, preprocessing, model training, and inference using gRPC. Demonstrates end-to-end ML workflow from PostgreSQL database through HDF5/CSV processing to trained model deployment.

Always reference these instructions first and fallback to search or bash commands only when you encounter unexpected information that does not match the info here.

## Working Effectively

### Prerequisites and Environment Setup
- **WINDOWS ONLY**: Download and install [Visual Studio Build Tools 2019/2022](https://aka.ms/vs/17/release/vs_BuildTools.exe)
- **Database**: Install [PostgreSQL](https://www.postgresql.org/download/)
  1. Download and install PostgreSQL
  2. Setup database: `psql -U postgres -f data/db/db.sql` 
  3. Create schema: `psql -U postgres -d iris -f data/db/ddl.sql`
  4. Load data: `psql -U postgres -d iris -f data/db/dml.sql`
- **Linux CI Alternative**: Uses CMake + Ninja with vcpkg (see CI configuration)

### Build Process (CRITICAL TIMING)
- **WINDOWS BUILD:**
  ```powershell
  scripts\build.ps1 -Configuration Debug   # or Release
  ```
  - **NEVER CANCEL: Build takes 25-45 minutes. WAIT FOR COMPLETION.**
  - **Set timeout to 60+ minutes minimum**
  - Dependencies: 29 packages including OpenBLAS, MLPack, gRPC, HDF5, PostgreSQL libs

- **LINUX BUILD (CI):**
  ```bash
  export VCPKG_ROOT=/usr/local/share/vcpkg
  cmake --preset CI
  cmake --build build --config Release
  ```
  - **NEVER CANCEL: Build takes 20-40 minutes. WAIT FOR COMPLETION.**
  - **Set timeout to 60+ minutes minimum**

### Database Validation
- Verify PostgreSQL is running: `pg_isready -h localhost -p 5432`
- Test iris database: `psql -U postgres -d iris -c "SELECT COUNT(*) FROM iris;"`
- Expected result: 150 records (Iris dataset)
- Validate data integrity: `psql -U postgres -d iris -c "SELECT specie, COUNT(*) FROM iris GROUP BY specie;"`
- Expected: 50 records each for Iris-setosa, Iris-versicolor, Iris-virginica

### Application Pipeline Execution
- **ALWAYS run in this exact sequence:**
  ```powershell
  scripts\run_service.ps1 -Configuration Debug   # or Release
  ```
- **Pipeline steps (automatic in run_service.ps1):**
  1. **Extractor**: PostgreSQL → HDF5 (`data/output.hdf`)
  2. **Wrangler**: HDF5 → CSV (`data/output.csv`) 
  3. **Trainer**: CSV → ML Model (`data/output.xml`) - **MUST achieve >90% accuracy**
  4. **Service**: Starts gRPC server on port 50051

### Manual Pipeline Execution
If run_service.ps1 fails, run components individually:
```powershell
# Step 1: Extract data
build\{preset}\extractor\{config}\extractor.exe -o data\output.hdf

# Step 2: Wrangle data  
build\{preset}\wrangler\{config}\wrangler.exe -i data\output.hdf -o data\output.csv

# Step 3: Train model (CRITICAL: must achieve >90% accuracy)
build\{preset}\trainer\{config}\trainer.exe -i data\output.csv -o data\output.xml

# Step 4: Start gRPC service
build\{preset}\service\{config}\service.exe -i data\output.xml
```

### Client Testing
- **Test gRPC service:**
  ```powershell
  build\{preset}\service\{config}\client.exe -p 50051
  ```
- **Expected output**: Classification results for hardcoded Iris samples
- **Service runs on**: localhost:50051

## Validation Scenarios

### MANDATORY Post-Change Validation
1. **Database connectivity test:**
   ```bash
   psql -U postgres -d iris -c "SELECT specie, COUNT(*) FROM iris GROUP BY specie;"
   ```
   Expected: 50 records each for Iris-setosa, Iris-versicolor, Iris-virginica

2. **Complete pipeline test:**
   - Run `scripts\run_service.ps1 -Configuration Debug`
   - Verify all 4 steps complete successfully
   - Check model accuracy is >90%
   - Service starts without errors

3. **gRPC client test:**
   - Start service in background
   - Run client.exe to test classifications
   - Verify responses contain valid species names

4. **File output validation:**
   - `data\output.hdf`: HDF5 file with species groups
   - `data\output.csv`: CSV with 150 rows + header
   - `data\output.xml`: MLPack model file

### Critical Failure Points
- **Model accuracy <90%**: Trainer will exit with failure
- **Missing PostgreSQL**: Extractor will fail to connect
- **Network issues**: vcpkg dependency downloads may fail
- **Insufficient disk space**: Large dependency builds require ~2GB
- **Port conflicts**: gRPC service requires port 50051 available

## Build Configurations

### Visual Studio Presets
- **VS17D**: Visual Studio 2022 Debug
- **VS17R**: Visual Studio 2022 Release  
- **VS16D**: Visual Studio 2019 Debug
- **VS16R**: Visual Studio 2019 Release
- **CI**: Linux Ninja Release (GitHub Actions)

### Key Directories
- **build/{preset}/**: Build outputs for specific configuration
- **data/**: Database scripts and pipeline outputs
  - **data/db/**: PostgreSQL setup scripts (db.sql, ddl.sql, dml.sql)
  - **data/output.hdf**: HDF5 file created by extractor
  - **data/output.csv**: CSV file created by wrangler  
  - **data/output.xml**: MLPack model file created by trainer
- **extractor/**: PostgreSQL to HDF5 converter (main.cpp, iris.h)
- **wrangler/**: HDF5 to CSV converter (main.cpp, Iris.h, CsvWriter.h)
- **trainer/**: MLPack SoftmaxRegression training (main.cpp)
- **service/**: gRPC server and client (main.cpp, client.cpp, ClassifierService.h)
  - **service/proto/**: Protocol buffer definitions (iris.proto)
- **scripts/**: PowerShell build and execution scripts

## Dependencies and Libraries
- **armadillo**: Linear algebra library
- **cli11**: Command line interface
- **fmt**: String formatting
- **highfive**: HDF5 C++ wrapper
- **mlpack**: Machine learning algorithms
- **sqlpp11**: PostgreSQL C++ connector
- **grpc**: RPC framework
- **protobuf**: Serialization

## Troubleshooting

### Build Issues
- **vcpkg timeout**: Network dependent, may require multiple attempts
- **Visual Studio not found**: Install Build Tools, verify with `vswhere.exe`
- **CMake not found**: Install with Visual Studio Build Tools
- **Compiler errors**: Ensure C++17 standard support

### Runtime Issues  
- **Database connection failed**: Verify PostgreSQL service running
- **File not found**: Check relative paths, ensure output directories exist
- **Model accuracy low**: Dataset may be corrupted, verify 150 records
- **gRPC connection refused**: Check if service is running on correct port

### Network Issues
- **sourceforge.net timeouts**: Common with some dependencies
- **GitHub clone failures**: Use HTTPS instead of SSH if needed
- **Proxy settings**: Configure for corporate environments

## Performance Expectations
- **Build time**: 25-45 minutes (NEVER CANCEL)
- **Full pipeline**: 30 seconds - 2 minutes  
- **Model training**: 5-15 seconds
- **gRPC startup**: <5 seconds
- **Client requests**: <100ms per classification

## Common Tasks and Quick Reference

### Rebuild After Code Changes
```powershell
# Clean and rebuild (Windows)
rmdir /s build
scripts\build.ps1 -Configuration Debug

# On Linux
rm -rf build  
export VCPKG_ROOT=/usr/local/share/vcpkg
cmake --preset CI && cmake --build build
```

### Database Management
```sql
-- Reset iris database
DROP DATABASE IF EXISTS iris;
CREATE DATABASE iris;
-- Then re-run setup scripts
```

### Pipeline Debug Commands
```powershell
# Test individual components with verbose output
build\{preset}\extractor\{config}\extractor.exe --help
build\{preset}\extractor\{config}\extractor.exe -H localhost -u iris -p iris -d iris -o data\debug.hdf

# Check file outputs
ls -la data\  # Linux
dir data\     # Windows

# Test gRPC connectivity  
netstat -an | findstr 50051  # Windows
netstat -an | grep 50051     # Linux
```

### Model Validation
The trainer component implements these validation rules:
- **Minimum accuracy**: 90% (ACCURACY_THRESHOLD in trainer/main.cpp)
- **Data split**: 80% training, 20% testing
- **Algorithm**: SoftmaxRegression from MLPack
- **Input features**: 4 (sepal length/width, petal length/width)
- **Output classes**: 3 (Setosa=1, Versicolor=2, Virginica=3)

Always allow extra time for long-running operations and verify all validation scenarios after making changes.