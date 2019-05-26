# IMAGE:    grpcservice
# VERSION:  1.0.0
# AUTHOR:   melon

FROM gcc

# This section for common define
ENV WORK_DIR /account
ENV LOG_DIR /data/log
ENV CONFIG_DIR /config
ENV DB_DIR /account/source/db


# This section for import code & config
RUN mkdir -p $WORK_DIR \
    && mkdir -p $LOG_DIR \
    && mkdir -p $CONFIG_DIR \
    && mkdir -p $DB_DIR

#RUN bazel build //examples:greeter_server

WORKDIR $WORK_DIR
COPY docker-src  $WORK_DIR/

EXPOSE 50051

#CMD ["sh", "greeter_server"]
