Ovato Core integration/staging tree
===================================

https://www.ovato.com

What is Ovato?
----------------

Ovato (OVO) blockchain technology, along with it’s strategic business plan is the key to providing a better way to exchange value in truly permissionless, non-custodial and decentralized economy. Ovato’s  (OVO) approach to mass adoption of its utility coin is razor focus and embracing centralized business principles and incentives to our decentralized currency.

Ovato brings conventional tried and true centralization of business principles to the decentralization technical core of it’s protocol. Ovato has created a full circle closed economy for it’s coin. The economy includes a variety of merchant and user wallets that sync directly to the OVO blockchain. This process provides users with all the benefits of decentralization while also providing all the advantages of scale and friendliness of a closed point to point solution.


For more information come to [ovato.com](https://ovato.com) and join our appropriate social networks

License
-------

Ovato Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/litecoin-project/litecoin/tags) are created
regularly to indicate new official, stable release versions of Ovato Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Useful hints for developers can be found in doc/developer-notes.md.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

We only accept translation fixes that are submitted through [Bitcoin Core's Transifex page](https://www.transifex.com/projects/p/bitcoin/).
Translations are converted to Ovato periodically.

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.
