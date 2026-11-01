#!/usr/bin/env node
// Portable tests: FizzBuzz unit tests, external library consumer, and
// a native CLI smoke test. No network, no service, no native UI.
const fs = require("node:fs"),
  path = require("node:path"),
  os = require("node:os");
const { spawnSync } = require("node:child_process");
const root = path.resolve(__dirname, "..");
const out = fs.mkdtempSync(path.join(os.tmpdir(), "altivec-host-"));
const cc = process.env.CC || "cc";
const flags = [
  "-std=c99",
  "-D_POSIX_C_SOURCE=200809L",
  "-Wall",
  "-Wextra",
  "-Werror",
  ...(process.env.SANITIZE === "1"
    ? ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
    : []),
];
function run(command, args) {
  const r = spawnSync(command, args, {
    cwd: root,
    encoding: "utf8",
    timeout: 60000,
  });
  if (r.error || r.status !== 0)
    throw Error(`${command}: ${r.error || r.stdout + r.stderr}`);
  return r.stdout;
}
function compile(name, sources) {
  run(cc, [
    ...flags,
    "-Isource/shared",
    ...sources,
    "-o",
    path.join(out, name),
  ]);
}
async function main() {
  // 1. Unit tests: compile and run the FizzBuzz test suite.
  compile("fizzbuzz-test", ["tests/model_test.c", "source/shared/model.c"]);
  run(path.join(out, "fizzbuzz-test"), []);

  // 2. External library consumer: compile the static library, link a consumer.
  run(cc, [
    ...flags,
    "-Isource/shared",
    "-c",
    "source/shared/model.c",
    "-o",
    path.join(out, "model.o"),
  ]);
  run("ar", ["rcs", path.join(out, "libmodel.a"), path.join(out, "model.o")]);
  run(cc, [
    ...flags,
    "-Isource/shared",
    "tests/consumer.c",
    path.join(out, "libmodel.a"),
    "-o",
    path.join(out, "consumer"),
  ]);
  const consumerOut = run(path.join(out, "consumer"), []);
  if (!consumerOut.includes("FizzBuzz"))
    throw Error("Consumer output missing FizzBuzz: " + consumerOut);
  if (!consumerOut.includes("1"))
    throw Error("Consumer output missing '1': " + consumerOut);

  // 3. Native CLI smoke test: build the full CLI and run it.
  compile("fizzbuzz-cli", ["source/cli/main.c", "source/shared/model.c"]);
  const cliOut = run(path.join(out, "fizzbuzz-cli"), []);
  const lines = cliOut.trim().split("\n");
  if (lines.length !== 1001)
    throw Error(`CLI produced ${lines.length} lines, expected 1001`);
  if (lines[0] !== "FizzBuzz")
    throw Error(`First line is '${lines[0]}', expected 'FizzBuzz'`);
  if (lines[1] !== "1")
    throw Error(`Second line is '${lines[1]}', expected '1'`);
  if (lines[2] !== "2")
    throw Error(`Third line is '${lines[2]}', expected '2'`);
  if (lines[3] !== "Fizz")
    throw Error(`Fourth line is '${lines[3]}', expected 'Fizz'`);
  if (lines[4] !== "4")
    throw Error(`Fifth line is '${lines[4]}', expected '4'`);
  if (lines[5] !== "Buzz")
    throw Error(`Sixth line is '${lines[5]}', expected 'Buzz'`);
  if (lines[1000] !== "Buzz")
    throw Error(`Last line is '${lines[1000]}', expected 'Buzz'`);

  console.log(
    "PASS: FizzBuzz unit tests, external library consumer, and native CLI smoke test",
  );
}
main()
  .catch((e) => {
    console.error(e.message);
    process.exitCode = 1;
  })
  .finally(() => fs.rmSync(out, { recursive: true, force: true }));
