FROM python:3.9

ADD . /app
WORKDIR /app

# Install chrome broswer
RUN curl -sS -o - https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add -
RUN echo "deb http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google-chrome.list
RUN apt-get -y update
RUN apt-get -y install google-chrome-stable python3-pip dbus dbus-x11 xvfb
RUN pip install --upgrade pip
RUN pip install selenium pyvirtualdisplay pandas

ENV DISPLAY=:0
RUN chmod 777 ./run.sh
CMD bash -C './run.sh'; 'bash'
