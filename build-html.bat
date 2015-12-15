@echo off
pandoc --css=styles.css -f markdown_github -t html -o iterator-facade-proposal.html iterator-facade-proposal.md
iterator-facade-proposal.html
