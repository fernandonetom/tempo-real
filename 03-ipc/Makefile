SUBDIRS += 01-pipe-simple
SUBDIRS += 02-pipe-stdio
SUBDIRS += 03-fifo
SUBDIRS += 04-socket
SUBDIRS += 05-shm
SUBDIRS += 06-msg-queues

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for i in  $(SUBDIRS); do \
		make -C $$i clean; \
	done \
