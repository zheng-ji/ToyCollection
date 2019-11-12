FROM python:3.6-alpine

ADD . /code

RUN cp /code/pip.conf /etc/

WORKDIR /code

RUN pip install -r requirements.txt

CMD ["python", "app.py"]
