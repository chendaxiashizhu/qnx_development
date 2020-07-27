//============================================================================
// Copyright (C) 2020 Xiaodan Tang
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//============================================================================
#include <errno.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include "../md5name_priv.h"

int md5_send(int fd, unsigned char *data, int len)
{
	md5_msg_t msg;
	iov_t iov[2];

	msg.msgtype = MD5_SEND_DATA;
	msg.msglen = len;
	SETIOV(&iov[0], &msg, sizeof(msg));
	SETIOV(&iov[1], data, len);
	return MsgSendv(fd, iov, 2, 0, 0);
}

int md5_recv(int fd, unsigned char *digest, int len)
{
	md5_msg_t msg;

	if (len < 16) {
		errno = EINVAL;
		return -1;
	}

	msg.msgtype = MD5_RECV_DIGEST;
	msg.msglen = len;
	return MsgSend(fd, &msg, sizeof(msg), digest, len);
}
