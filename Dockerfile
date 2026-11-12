FROM gcc:13-alpine

WORKDIR /app
COPY Makefile ./
COPY include/ ./include/
COPY src/ ./src/
COPY tests/ ./tests/

RUN make && make test

RUN rm -f test_memory_pool *.o

CMD ["make", "run"]