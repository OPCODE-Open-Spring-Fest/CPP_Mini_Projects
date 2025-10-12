import { getOctokit, context } from '@actions/github';
import { setFailed } from '@actions/core';

async function run() {
  try {
    const token = process.env.GITHUB_TOKEN;
    const octokit = new getOctokit(token);

    const pullRequest = context.payload.pull_request;
    const owner = pullRequest.base.repo.owner.login;
    const repo = pullRequest.base.repo.name;
    const pullNumber = pullRequest.number;

    const { data: labels } = await octokit.rest.issues.listLabelsOnIssue({
      owner,
      repo,
      issue_number: pullNumber,
    });

    const labelNames = labels.map((label) => label.name);

    const requiredLabels = [
      ['Type:Easy', 'Type:Medium', 'Type:Hard'],
      ['Semver:major', 'Semver:minor', 'Semver:patch'],
      ['PR:Accept'],
    ];

    const hasRequiredLabels = requiredLabels.every((labelGroup) =>
      labelGroup.some((label) => labelNames.includes(label))
    );

    if (!hasRequiredLabels) {
      setFailed(
        'This pull request must have at least one label from each of the following groups: Type (Easy, Medium, Hard), Semver (Major, Minor, Patch), and PR:Accept.'
      );
    }
  } catch (error) {
    setFailed(error.message);
  }
}

run();