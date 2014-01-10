# create tex
pandoc --template technical_specification_working_draft.latex -H header.tex --number-sections --table-of-contents parallelism.*.md -o parallelism.tex

# annotate headings with labels
perl -i -p00e 's/\\section{(.*)}\\label{(.*)}/\\Sec0[\2]{\1}/s' parallelism.tex
perl -i -p00e 's/\\subsection{(.*)}\\label{(.*)}/\\Sec1[\2]{\1}/s' parallelism.tex
perl -i -p00e 's/\\subsubsection{(.*)}\\label{(.*)}/\\Sec2[\2]{\1}/s' parallelism.tex
perl -i -p00e 's/\\paragraph{(.*)}\\label{(.*)}/\\Sec3[\2]{\1}/s' parallelism.tex
perl -i -p00e 's/\\subparagraph{(.*)}\\label{(.*)}/\\Sec4[\2]{\1}/s' parallelism.tex

# create pdf
pdflatex parallelism.tex

