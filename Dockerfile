FROM ubuntu:22.04 as builder

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsqlite3-dev \
    libjsoncpp-dev \
    git \
    uuid-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code (excluding build directory)
COPY . .

# Remove existing build directory if it exists
RUN rm -rf build

# Build application - use the simple API implementation only
RUN mkdir -p build && \
    cd build && \
    cmake -DBUILD_TESTING=OFF -DBUILD_API=OFF .. && \
    cmake --build . -j $(nproc)

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libsqlite3-0 \
    libjsoncpp25 \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -m appuser

# Set working directory
WORKDIR /app

# Create data directory
RUN mkdir -p /app/data

# Copy built executable and necessary files from builder
COPY --from=builder /app/build/todo_api_simple /app/todo_api
COPY --from=builder /app/data/ /app/data/

# Change ownership
RUN chown -R appuser:appuser /app

# Switch to non-root user
USER appuser

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
  CMD curl -f http://localhost:8080/health || exit 1

# Run application
EXPOSE 8080
CMD ["/app/todo_api"]
