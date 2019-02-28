# -*- python -*-

from omniidl_be.python import *
from omniidl_be.python import run as run_parent
from omniidl import idlvisitor, idlast, idltype

def _rreplace(s, old, new, occurrence):
    li = s.rsplit(old, occurrence)
    return new.join(li)

class CommentToConstVisitor (idlvisitor.AstVisitor):
    def __init__(self):
        import re
        self.commentStart  = re.compile (r"^//+ ?")

    def _commentToConst (self, node, comments):
        import re
        texts =  []
        line = node.line()
        # TODO here, we only use the comments before.
        comment = node
        for c in reversed(comments):
            if line - c.line() != 1:
                break
            line = c.line()
            comment = c
            text = c.text()
            texts.append (re.sub(self.commentStart, "", text))
        if len(texts)==0:
            print (node.identifier() + " documentation may be ill-formed.")
            return None
        texts.reverse()
        text = ''.join (texts)
        id = node.identifier() + "__doc__"
        return idlast.Const (
                comment.file(),
                comment.line(),
                node.mainFile(),
                node.pragmas(),
                [], # comments
                id, #identifier
                node.scopedName()[:-1] + [ id, ], #scopedName
                _rreplace (node.repoId(), node.identifier(), id, 1), # repoId
                idltype.String (0), # constType, 0 means unbounded
                idltype.tk_string, # constKind
                text # value
                )
    def _addDoc (self, parent, node):
        if len(node.comments()) > 0:
            const = self._commentToConst (node, node.comments())
            if const is None: return
            if isinstance(parent, idlast.Module):
                parent._Module__definitions.append(const)
            elif isinstance(parent, idlast.Interface):
                parent._Interface__declarations.append(const)
                parent._Interface__contents.append(const)
            elif isinstance(parent, idlast.AST):
                parent._AST__declarations.append(const)
            else:
                print ("Doc ignored: " + comment.text())


    def visitAST(self, node):
        for n in node.declarations():
            if not output_inline and not n.mainFile(): continue

            if isinstance(n, idlast.Module) or isinstance(n, idlast.Interface):
                self._addDoc (node, n)
                n.accept(self)

    def visitModule(self, node):
        for n in node.definitions():
            if not output_inline and not n.mainFile(): continue

            if isinstance(n, idlast.Module) or isinstance(n, idlast.Interface):
                self._addDoc (node, n)
                n.accept(self)

    def visitInterface(self, node):
        for c in node.callables():
            self._addDoc (node, c)

def run(tree, args):
    ccv = CommentToConstVisitor ()
    tree.accept(ccv)

    run_parent (tree, args)