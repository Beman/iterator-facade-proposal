@echo off
pandoc --css=styles.css --toc --toc-depth=4 -f markdown_github -t html -o iterator-facade-proposal.html iterator-facade-proposal.md
iterator-facade-proposal.html
