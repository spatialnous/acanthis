# SPDX-FileCopyrightText: 2018 Christian Sailer
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# REUSE-IgnoreStart

rm version_defs.h
printf "// SPDX-FileCopyrightText: Christian Sailer\n" >> version_defs.h
printf "//"
printf "// SPDX-License-Identifier: GPL-3.0-or-later\n\n" >> version_defs.h

printf "// This file is autogenerated - do not modify it directly!\n\n" >> version_defs.h

printf "#pragma once\n\n" >> version_defs.h

printf "#ifndef APP_DATE\n" >> version_defs.h
printf "#define APP_DATE \"`date +%Y/%m/%d`\"\n" >> version_defs.h
printf "#endif\n\n" >> version_defs.h

printf "#ifndef APP_GIT_BRANCH\n" >> version_defs.h
printf "#define APP_GIT_BRANCH \"`git rev-parse --abbrev-ref HEAD`\"\n" >> version_defs.h
printf "#endif\n\n" >> version_defs.h

printf "#ifndef APP_GIT_COMMIT\n" >> version_defs.h
printf "#define APP_GIT_COMMIT \"`git log --pretty=format:'%h' -n 1`\"\n" >> version_defs.h
printf "#endif\n" >> version_defs.h

