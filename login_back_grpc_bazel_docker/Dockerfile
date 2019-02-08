# IMAGE:    grpcservice
# VERSION:  1.0.0
# AUTHOR:   melon

FROM gcc

#MAINTAINER Weiwen Tu <tuweiwen@ipin.com>

# This section for common define
ENV WORK_DIR /account
ENV LOG_DIR /data/log
ENV CONFIG_DIR /config



# This section for import code & config
RUN mkdir -p $WORK_DIR \
    && mkdir -p $LOG_DIR \
    && mkdir -p $CONFIG_DIR

#RUN bazel build //examples:greeter_server

WORKDIR $WORK_DIR
COPY docker-src  $WORK_DIR/

EXPOSE 50051

#CMD ["sh", "greeter_server"]
