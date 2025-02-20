[% USE ItemTypes %]
[% USE AuthorisedValues %]
<table id="[% table_name | html %]">
    <thead>
        <tr>
            <th class="title-string">Waiting since</th>
            <th class="title-string">Date hold placed</th>
            <th class="anti-the">Title</th>
            <th>Patron</th>
            <th>Home library</th>
            <th>Current location</th>
            <th>Shelving location</th>
            <th>Call number</th>
            <th>Copy number</th>
            <th>Enumeration</th>
            <th>Action</th>
        </tr>
    </thead>
    <tbody>
        [% FOREACH reserveloo IN reserveloop %]
            <tr>
                <td><span title="[% reserveloo.waitingdate | html %]">[% reserveloo.waitingdate | $KohaDates %]</span></td>
                <td><span title="[% reserveloo.reservedate | html %]">[% reserveloo.reservedate | $KohaDates %]</span></td>
                <td>[% INCLUDE 'biblio-default-view.inc' biblionumber = reserveloo.biblionumber %]
                        [% INCLUDE 'biblio-title.inc' biblio=reserveloo.biblio %]
                    </a>
                        [% UNLESS ( item_level_itypes ) %]
                            [% IF ( ItemTypes.GetDescription(reserveloo.item.effective_itemtype) ) %]&nbsp; (<b>[% ItemTypes.GetDescription(reserveloo.item.effective_itemtype) | html %]</b>)
                            [% END %]
                        [% END %]
                        <br />Barcode: [% reserveloo.item.barcode | html %]
                </td>
                <td><a href="/cgi-bin/koha/members/moremember.pl?borrowernumber=[% reserveloo.borrower.borrowernumber | uri %]">[% INCLUDE 'patron-title.inc' patron=reserveloo.borrower invert_name=1 %]</a>
                    [% IF ( reserveloo.borrower.phone ) %]<br /><span class="patron_phone">[% reserveloo.borrower.phone | html %]</span>[% END %]
                    [% IF ( reserveloo.borrower.first_valid_email_address ) %]
                        <span class="patron_email"><br /><a href="mailto:[% reserveloo.borrower.first_valid_email_address | uri %]?subject=[% "Hold waiting: " | uri %][% reserveloo.biblio.title | uri %]">
                        [% reserveloo.borrower.first_valid_email_address | html %]</a></span>
                    [% END %]
                </td>
                <td>[% Branches.GetName( reserveloo.item.homebranch ) | html %]</td>
                <td>[% Branches.GetName( reserveloo.item.holdingbranch ) | html %]</td>
                <td>[% AuthorisedValues.GetByCode('LOC', reserveloo.item.location) | html %]</td>
                <td>[% reserveloo.item.itemcallnumber | html %]</td>
                <td>[% reserveloo.item.copynumber | html %]</td>
                <td>[% reserveloo.item.enumchron | html %]</td>
                <td>
                    <form name="cancelReserve" action="waitingreserves.pl" method="post">
                        <input type="hidden" name="borrowernumber" value="[% reserveloo.borrower.borrowernumber | html %]" />
                        <input type="hidden" name="itemnumber" value="[% reserveloo.item.itemnumber | html %]" />
                        <input type="hidden" name="fbr" value="[% reserveloo.item.holdingbranch | html %]" />
                        <input type="hidden" name="tbr" value="[% reserveloo.item.homebranch | html %]" />
                        <input type="hidden" name="tab" value="[% tab | html %]">
                        [% IF ( reserveloo.item.homebranch != reserveloo.item.holdingbranch ) %]
                            <button type="submit" class="btn btn-default btn-xs"><i class="fa fa-remove"></i> Cancel hold and return to: [% Branches.GetName( reserveloo.item.homebranch ) | html %]</button>
                        [% ELSE %]
                            <button type="submit" class="btn btn-default btn-xs"><i class="fa fa-remove"></i> Cancel hold</button>
                        [% END %]
                   </form>
                </td>
            </tr>
        [% END %]
    </tbody>
</table>
