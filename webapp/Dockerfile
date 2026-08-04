FROM python:3.11-slim

# Install system dependencies
RUN apt-get update && apt-get install -y \
    ffmpeg \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Upgrade pip and install a safe setuptools version first
RUN pip install --no-cache-dir --upgrade pip setuptools==69.5.1 wheel

# Copy requirements
COPY requirements.txt .

# Install requirements without build isolation so it uses the pinned setuptools
RUN pip install --no-cache-dir --no-build-isolation -r requirements.txt

COPY . .

EXPOSE 5000

CMD ["python", "run.py"]