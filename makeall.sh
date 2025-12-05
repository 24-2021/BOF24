#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET="${1:-all}"

MODS_DIR="${ROOT_DIR}/bof24"

if [ ! -d "${MODS_DIR}" ]; then
  echo "bof24 目录不存在: ${MODS_DIR}" >&2
  exit 1
fi

built=()
failed=()

for dir in "${MODS_DIR}"/*; do
  [ -d "${dir}" ] || continue
  name="$(basename "${dir}")"
  [ "${name}" = "base_template" ] && continue
  if [ -f "${dir}/Makefile" ]; then
    echo "==> ${name}: make ${TARGET}"
    if make -C "${dir}" "${TARGET}"; then
      built+=("${name}")
    else
      failed+=("${name}")
    fi
  fi
done

echo ""
echo "完成: ${#built[@]} 成功, ${#failed[@]} 失败"
if [ ${#built[@]} -gt 0 ]; then
  printf "成功: %s\n" "${built[@]}"
fi
if [ ${#failed[@]} -gt 0 ]; then
  printf "失败: %s\n" "${failed[@]}"
  exit 1
fi

