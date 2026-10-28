import re

with open("docs/milestones.md", "r") as f:
    content = f.read()

total = len(re.findall(r"\| M\d+\s+\|", content))
completed = len(re.findall(r"\| COMPLETE\s+\|", content))

print(f"Total: {total}, Completed: {completed}")
