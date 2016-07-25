FROM ubuntu:utopic
MAINTAINER caktux

ENV DEBIAN_FRONTEND noninteractive

# Usual update / upgrade
RUN apt-get update
RUN apt-get upgrade -q -y
RUN apt-get dist-upgrade -q -y

# Let our containers upgrade themselves
RUN apt-get install -q -y unattended-upgrades

# Install Elementrem
RUN apt-get install -q -y software-properties-common
RUN add-apt-repository ppa:elementrem/elementrem
RUN add-apt-repository ppa:elementrem/elementrem-dev
RUN apt-get update
RUN apt-get install -q -y ele

# Install supervisor
RUN apt-get install -q -y supervisor

# Add supervisor configs
ADD supervisord.conf supervisord.conf

EXPOSE 8080
EXPOSE 30707

CMD ["-n", "-c", "/supervisord.conf"]
ENTRYPOINT ["/usr/bin/supervisord"]
