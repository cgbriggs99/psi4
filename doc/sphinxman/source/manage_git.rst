.. #
.. # @BEGIN LICENSE
.. #
.. # Psi4: an open-source quantum chemistry software package
.. #
.. # Copyright (c) 2007-2017 The Psi4 Developers.
.. #
.. # The copyrights for code used from other parties are included in
.. # the corresponding files.
.. #
.. # This file is part of Psi4.
.. #
.. # Psi4 is free software; you can redistribute it and/or modify
.. # it under the terms of the GNU Lesser General Public License as published by
.. # the Free Software Foundation, version 3.
.. #
.. # Psi4 is distributed in the hope that it will be useful,
.. # but WITHOUT ANY WARRANTY; without even the implied warranty of
.. # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
.. # GNU Lesser General Public License for more details.
.. #
.. # You should have received a copy of the GNU Lesser General Public License along
.. # with Psi4; if not, write to the Free Software Foundation, Inc.,
.. # 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
.. #
.. # @END LICENSE
.. #

.. include:: autodoc_abbr_options_c.rst

.. .. _`sec:addAddOns`:

Git, Versioning
===============

.. _`faq:versionbump`:

How to bump a version
---------------------

0. **ACT** to check everything in

1. **OBSERVE** current versioning state

  * Be on master of (i) a direct clone or (ii) clone-of-fork with master
    up-to-date with upstream (including tags!!!) and with upstream as
    remote.

  * https://github.com/psi4/psi4/releases says ``v1.1a1`` & ``007a9b6``

  ::

    >>> git tag
    v1.0
    v1.1a1

    >>> cat psi4/metadata.py
    __version__ = '1.1a1'
    __version_long = '1.1a1+007a9b6'
    __version_upcoming_annotated_v_tag = '1.1a2'

    >>> git describe --abbrev=7 --long --always HEAD
    v1.1a1-417-gcbee32b

    >>> git describe --abbrev=7 --long --dirty
    v1.1a1-417-gcbee32b

    >>> ./psi4/versioner.py
    Defining development snapshot version: 1.1a2.dev417+cbee32b (computed)
    1.1a2.dev417 {master} cbee32b 1.0.0.999   1.0 <-- 1.1a2.dev417+cbee32b

    >>> git diff

  * Observe that current latest tag matches metadata scipt and git
    describe, that GH releases matches metadata script, that upcoming in
    metadata script matches current versioner version.

  * Note that current tag is ``v1.1a1``. Decide on imminent tag, say ``v1.1rc1``.

2. **ACT** to bump tag in code

  * Edit current & prospective tag in :source:`psi4/metadata.py`. Use your
    decided-upon tag ``v1.1rc1`` and a speculative next tag, say ``v1.1rc2``,
    and use 7 "z"s for the part you can't predict.

  ::

    >>> vi psi4/metadata.py

    >>> git diff
    diff --git a/psi4/metadata.py b/psi4/metadata.py
    index 5d87b55..6cbc05e 100644
    --- a/psi4/metadata.py
    +++ b/psi4/metadata.py
    @@ -1,6 +1,6 @@
    -__version__ = '1.1a1'
    -__version_long = '1.1a1+007a9b6'
    -__version_upcoming_annotated_v_tag = '1.1a2'
    +__version__ = '1.1rc1'
    +__version_long = '1.1rc1+zzzzzzz'
    +__version_upcoming_annotated_v_tag = '1.1rc2'

    >>> git add psi4/metadata.py

    >>> git commit -m "v1.1rc1"

3. **OBSERVE** undefined version state

  ::

    >>> git describe --abbrev=7 --long --always HEAD
    v1.1a1-418-g6100822

    >>>  git describe --abbrev=7 --long --dirty
    v1.1a1-418-g6100822

    >>>  psi4/versioner.py
    Undefining version for irreconcilable tags: 1.1a1 (computed) vs 1.1rc1 (recorded)
    undefined {master} 6100822 1.0.0.999   1.0 <-- undefined+6100822

  * Note 7-char git hash for the new commit, here "6100822".

4. **ACT** to bump tag in git, then bump git tag in code.

  * Use the decided-upon tag ``v1.1rc1`` and the observed hash "6100822" to
    mint a new *annotated* tag, minding that "v"s are present here.

  * Use the observed hash to edit :source:`psi4/metadata.py` and commit immediately.

  ::

    >>> git tag -a v1.1rc1 6100822 -m "v1.1rc1"

    >>> vi psi4/metadata.py
    >>> git diff
    diff --git a/psi4/metadata.py b/psi4/metadata.py
    index 6cbc05e..fdc202e 100644
    --- a/psi4/metadata.py
    +++ b/psi4/metadata.py
    @@ -1,5 +1,5 @@
     __version__ = '1.1rc1'
    -__version_long = '1.1rc1+zzzzzzz'
    +__version_long = '1.1rc1+6100822'
     __version_upcoming_annotated_v_tag = '1.1rc2'

    >>> psi4/versioner.py
    Amazing, this can't actually happen that git hash stored at git commit.
    >>> git add psi4/metadata.py
    >>> git commit -m "Records tag for v1.1rc1"

5. **OBSERVE** current versioning state

  * Nothing to make note of, this is just a snapshot.

  ::

    >>> psi4/versioner.py
    Defining development snapshot version: 1.1rc2.dev1+4e0596e (computed)
    1.1rc2.dev1 {master} 4e0596e 1.0.0.999   1.0 <-- 1.1rc2.dev1+4e0596e

    >>> git describe --abbrev=7 --long --always HEAD
    v1.1rc1-1-g4e0596e

    >>> git describe --abbrev=7 --long --dirty
    v1.1rc1-1-g4e0596e

    >>> git tag
    v1.0
    v1.1a1
    v1.1rc1

    >>> cat psi4/metadata.py
    __version__ = '1.1rc1'
    __version_long = '1.1rc1+6100822'
    __version_upcoming_annotated_v_tag = '1.1rc2'

    >>> cat metadata.out.py | head -8
    __version__ = '1.1rc2.dev1'
    __version_branch_name = 'master'
    __version_cmake = '1.0.0.999'
    __version_is_clean = 'True'
    __version_last_release = '1.0'
    __version_long = '1.1rc2.dev1+4e0596e'
    __version_prerelease = 'False'
    __version_release = 'False'

    >>> git log --oneline
    4e0596e Records tag for v1.1rc1
    6100822 v1.1rc1
    cbee32b Fixes pcmsolver/scf for py3. Moves source for libefp upstream.

6. **ACT** to inform remote of bump

  * Temporarily disengage "Include administrators" on protected master branch.

  ::

    >>> git push origin master
    >>> git push origin v1.1rc1

  * Now https://github.com/psi4/psi4/releases says ``v1.1rc1`` & ``6100822``


.. _`faq:remotetag`:

How to create and remove an annotated Git tag on a remote
---------------------------------------------------------

|PSIfour| versioning only works with *annotated* tags, not *lightweight*
tags as are created with the `GitHub interface
<https://github.com/psi4/psi4/releases/new>`_

* Create *annotated* tag::

    >>> git tag -a v1.1a1 <git hash if not current> -m "v1.1a1"
    >>> git push origin v1.1a1

* Delete tag::

    >>> git tag -d v1.1a1
    >>> git push origin :refs/tags/v1.1a1

* Pull tags::

    >>> git fetch <remote> 'refs/tags/*:refs/tags/*'

.. _`faq:psi4version`:

What Psi4 version is running
----------------------------

* Psithon / from the executable::

    >>> psi4 --version
    1.1rc2.dev17

* PsiAPI / from the library::

    >>> python -c "import psi4; print(psi4.__version__)"
    1.1rc2.dev17

* Output file header gives info like the ``print_header()`` below.

* Function ``print_header()`` returns a summary of citation, version, and
  git information about |PSIfour|. Function ``version_formatter()`` can
  return version and git information in any desired format string. ::

    >>> import psi4
    >>> psi4.print_header()

        -----------------------------------------------------------------------
              Psi4: An Open-Source Ab Initio Electronic Structure Package
                                   Psi4 1.1rc2.dev17

                             Git: Rev {condadoc} c852257 dirty


        R. M. Parrish, L. A. Burns, D. G. A. Smith, A. C. Simmonett,
        A. E. DePrince III, E. G. Hohenstein, U. Bozkaya, A. Yu. Sokolov,
        R. Di Remigio, R. M. Richard, J. F. Gonthier, A. M. James,
        H. R. McAlexander, A. Kumar, M. Saitow, X. Wang, B. P. Pritchard,
        P. Verma, H. F. Schaefer III, K. Patkowski, R. A. King, E. F. Valeev,
        F. A. Evangelista, J. M. Turney, T. D. Crawford, and C. D. Sherrill,
        submitted.

        -----------------------------------------------------------------------


        Psi4 started on: Friday, 28 April 2017 07:31PM

        Process ID:  95107
        PSIDATADIR: /Users/loriab/linux/psihub/hrw-labfork/objdir8/stage/usr/local/psi4/share/psi4
        Memory:     500.0 MiB
        Threads:    1

    >>> psi4.version_formatter()
    '1.1rc2.dev17'
    >>> psi4.version_formatter('all')
    '1.1rc2.dev17 {condadoc} c852257 1.0.0.999 dirty  1.0 <-- 1.1rc2.dev17+c852257'
    >>> psi4.version_formatter("""{{{branch}}} {versionlong}""")
    '{condadoc} 1.1rc2.dev17+c852257'

