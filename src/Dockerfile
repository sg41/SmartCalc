FROM ubuntu:20.04

HEALTHCHECK --interval=30s --timeout=2s --start-period=5s --retries=3 CMD curl -f http://localhost/ || exit 1

# Fix timezone issue
ENV TZ=Asia/Novosibirsk
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get -y update && apt-get -y install --no-install-recommends \
    make \
    gcc \
    libgtk-3-0 \
    libgtk-3-bin \
    libgtk-3-dev \
    pkg-config \
    valgrind\
    gcovr \
    check \
    texinfo \
    texlive-binaries \
    && apt-get clean && rm -rf /var/lib/apt/lists
WORKDIR /s21_Smart_Calc_1.0/src
COPY ./  ./

RUN make install

# COPY ./nginx/nginx.conf /etc/nginx/nginx.conf
# RUN  chmod u-s \
#     /usr/bin/chsh\
#     /usr/bin/chfn\
#     /usr/bin/passwd\
#     /bin/umount\
#     /bin/su\
#     /bin/mount\
#     /usr/bin/gpasswd\
#     /usr/bin/newgrp &&\
#     chmod g-s \
#     /usr/bin/chage\
#     /usr/bin/expiry\
#     /sbin/unix_chkpwd\
#     /usr/bin/wall &&\
#     chown -R nginx:nginx /var/cache/nginx/ && \
#     chown -R nginx:nginx /var/log/nginx/

# USER nginx:nginx

ENTRYPOINT ["./run.sh"]
