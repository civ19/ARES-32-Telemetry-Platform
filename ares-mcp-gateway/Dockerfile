FROM python:3.11-slim

WORKDIR /app

COPY requirements.txt .
RUN pip install --nocache-dir -r requirements.txt

COPY server.py .

EXPOSE 8000

ENTRYPOINT ["python", "server.py"]

