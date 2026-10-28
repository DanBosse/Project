import re

with open("docs/milestones.md", "r") as f:
    content = f.read()

total = len(re.findall(r"\| M\d+\s+\|", content))
completed = len(re.findall(r"\| COMPLETE\s+\|", content))
pct = int((completed / total) * 100) if total > 0 else 0

bar = "[" + "█" * int(pct / 2) + "░" * (50 - int(pct / 2)) + "]"
snippet = f"### Core Platform Readiness\n`{bar}` **{pct}%** ({completed}/{total} Milestones Complete)"

with open("README.md", "r") as f:
    readme = f.read()

pattern = r"(<!--START_SECTION:milestones-->)(.*?)(<!--END_SECTION:milestones-->)"
if re.search(pattern, readme, re.DOTALL):
    updated = re.sub(pattern, f"\\1\n{snippet}\n\\3", readme, flags=re.DOTALL)
else:
    updated = readme + f"\n\n<!--START_SECTION:milestones-->\n{snippet}\n<!--END_SECTION:milestones-->\n"

with open("README.md", "w") as f:
    f.write(updated)

print(f"Successfully updated README.md: {pct}% readiness.")
