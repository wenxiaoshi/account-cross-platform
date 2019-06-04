# IMAGE:    grpcservice
# VERSION:  1.0.0
# AUTHOR:   melon

FROM gcc:1.0.1

# This section for common define
ENV WORK_DIR /
ENV LOG_DIR /data/log
ENV CONFIG_DIR /source/conf
ENV DB_DIR /source/db
ENV PEM_DIR /source/pem

# This section for import code & config
RUN mkdir -p $WORK_DIR \
    && mkdir -p $LOG_DIR \
    && mkdir -p $CONFIG_DIR \
    && mkdir -p $DB_DIR \
    && mkdir -p $PEM_DIR

#RUN bazel build //examples:greeter_server

WORKDIR $WORK_DIR
COPY docker-src  $WORK_DIR/

EXPOSE 50051

#CMD ["sh", "greeter_server"]
