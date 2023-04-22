#!/usr/bin/python3
import argparse
import json
import re
import subprocess

GIT_EXPRESSION = r"^([a-zA-Z0-9\-_\\.]+)-([0-9]+)-g([a-zA-Z0-9]+)$"
SEMVER_EXPRESSION = r"^(?P<major>0|[1-9]\d*)\.(?P<minor>0|[1-9]\d*)\.(?P<patch>0|[1-9]\d*)(?:-(?P<prerelease>(?:0|[" \
                    r"1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(" \
                    r"?P<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$"


def is_git_clean():
    invocation = subprocess.run(
        ["git", "status", "--short"],
        capture_output=True
    )
    return len(invocation.stdout) == 0


def extract_simple_data(git_description, include_match: bool = False):
    match = re.match(GIT_EXPRESSION, git_description)
    if match is None:
        raise Exception(
            f"Unable to find version using git describe. (git_description: '{git_description}', expression: '{GIT_EXPRESSION}')",
            git_description
        )
    version = match.group(1).removeprefix('v')
    raw = version
    semver_regex = re.match(SEMVER_EXPRESSION, raw)

    response = {
        "raw": raw,
        "major": semver_regex.group("major"),
        "minor": semver_regex.group("minor"),
        "patch": semver_regex.group("patch")
    }
    if include_match:
        response["match"] = match
    return response


def find_version_simple(include_match: bool = False):
    return extract_simple_data(git_describe(), include_match)


def find_version():
    response = find_version_simple(True)
    match = response.pop("match")

    version = match.group(1).removeprefix('v')
    num_commits_ahead = int(match.group(2))
    revision = match.group(3)

    build_metadata = list()
    if num_commits_ahead > 0:
        build_metadata.append(revision)

    if not is_git_clean():
        build_metadata.append("dirty")

    raw = build_version(build_metadata, version)
    semver_regex = re.match(SEMVER_EXPRESSION, raw)

    response["raw"] = raw
    response["prerelease"] = semver_regex.group("prerelease")
    response["build_metadata"] = semver_regex.group("buildmetadata")

    return response


def build_version(build_metadata, version):
    final_version = version
    if len(build_metadata) > 0:
        metadata = '-'.join(build_metadata)
        final_version += f"+{metadata}"
    return final_version


def git_describe():
    invocation = subprocess.run(["git", "describe", "--tags", "--always", "--long"],
                                capture_output=True)
    if invocation.returncode != 0:
        raise Exception(
            f"Git invocation failed with return code {invocation.returncode}: {str(invocation.stderr, 'utf-8')}")

    version_str = str(invocation.stdout, 'utf-8').removesuffix('\n')
    return version_str


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--json", required=False, action='store_true')

    args = parser.parse_args()

    versionContent = find_version()

    if args.json:
        print(json.dumps(versionContent))
    else:
        print(versionContent['raw'])
