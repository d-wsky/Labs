# 
# Copyright (c) 2016, Denis Vasilkovskii
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#      - Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      - Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      - Neither the name of the OINPE NRNU MEPhI nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
# Makefile
#
# Created: 01-Oct-16 21:32:40
#  Author: Denis Vasilkovskii
#
# Disclaimer: All comments are in Windows CP1251 (not UTF8) to guarantee support
# for all make versions.

# �������� ���� �� �������� �����������
include Makefile.in

# � ������� ���� ���� ����� ��������� ���������� LabLib ��� ��
all:
	$(Q)$(MAKE) -C $(LIB_NAME)

# ������� ���� ���������� �� �������� � ������������� ������ ����������
clean: clean_unit
	$(Q)$(MAKE) clean -C $(LIB_NAME)

# ���� ��� ���������� ����-������
unit:
	$(Q)$(MAKE) all -C $(UNIT_DIR)

clean_unit:
	$(Q)$(MAKE) clean -C $(UNIT_DIR)

# ���� ��� ������� ����-������
check:
	$(Q)$(MAKE) check -C $(UNIT_DIR)