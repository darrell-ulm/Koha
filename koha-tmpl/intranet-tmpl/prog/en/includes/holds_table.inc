[% USE Koha %]
<table>
    <tr>
        [% IF ( CAN_user_reserveforothers_modify_holds_priority ) %]
            <th>Priority</th>
            <th>&nbsp;</th>
        [% ELSE %]
            <th>Delete?</th>
        [% END %]
        <th>Patron</th>
        <th>Notes</th>
        <th>Date</th>
        <th>Expiration</th>
        <th>Pickup library</th>
        <th>Details</th>
        [% IF ( CAN_user_reserveforothers_modify_holds_priority ) %]
            <th><img src="[% interface | html %]/[% theme | html %]/img/go-bottom.png" alt="Toggle set to lowest priority" /></th>
        [% END %]
        <th>&nbsp;</th>
        <th>&nbsp;</th>
    </tr>

    [% SET first_priority = 0 %]
    [% SET last_priority  = holds.last.priority %]

    [% FOREACH hold IN holds %]
    [% IF !hold.found && first_priority == 0 %][% first_priority = hold.priority %][% END %]
        <tr>
            <td>
                <input type="hidden" name="reserve_id" value="[% hold.reserve_id | html %]" />
                <input type="hidden" name="borrowernumber" value="[% hold.borrowernumber | html %]" />
                <input type="hidden" name="biblionumber" value="[% hold.biblionumber | html %]" />
                [% IF Koha.Preference('HoldsSplitQueue') == "nothing" && !hold.found %]
                    <select name="rank-request">
                [% ELSE %]
                    <input type="hidden" name="rank-request" value="[% hold.priority | html %]">
                    <select name="rank-request" disabled="disabled">
                [% END %]
                    [% IF ( hold.found ) %]
                        [% IF ( hold.intransit ) %]
                            <option value="T" selected="selected">In transit</option>
                        [% ELSE %]
                            <option value="W" selected="selected">Waiting</option>
                        [% END %]
                    [% END %]

                    [% IF ( CAN_user_reserveforothers_modify_holds_priority ) %]
                        [% IF Koha.Preference('HoldsSplitQueueNumbering') == 'actual' %]
                            [% FOREACH optionloo IN hold.optionloop %]
                                [% IF ( optionloo.selected ) %]
                                    <option value="[% optionloo.num | html %]" selected="selected">[% optionloo.num | html %]</option>
                                [% ELSE %]
                                    <option value="[% optionloo.num | html %]">[% optionloo.num | html %]</option>
                                [% END %]
                            [% END %]
                        [% ELSE %]
                            [% SET ranker = 1 %]
                            [% FOREACH h IN holds %]
                                [% NEXT IF h.found %]
                                [% IF ( h.priority == hold.priority ) %]
                                    <option value="[% h.priority | html %]" selected="selected">[% ranker | html %]</option>
                                [% ELSE %]
                                    <option value="[% h.priority | html %]">[% ranker | html %]</option>
                                [% END %]
                                [% ranker = ranker + 1 %]
                            [% END %]
                        [% END %]
                    [% ELSIF !hold.found %]
                        <option value="[% hold.priority | html %]" selected="selected">[% hold.priority | html %]</option>
                    [% END %]

                    <option value="del">del</option>
                </select>
            </td>

            [% IF ( CAN_user_reserveforothers_modify_holds_priority ) %]
            [% UNLESS hold.found %]
                    [% SET prev_priority  = loop.prev.priority %]
                    [% SET next_priority  = loop.next.priority %]
                    [% holds.index | html %]

                    <td style="white-space:nowrap;">
                        <a title="Move hold up" href="request.pl?action=move&amp;where=up&amp;first_priority=[% first_priority | html %]&amp;last_priority=[% last_priority | html %]&amp;prev_priority=[% prev_priority | html %]&amp;next_priority=[% next_priority | html %]&amp;borrowernumber=[% hold.borrowernumber | html %]&amp;biblionumber=[% hold.biblionumber | html %]&amp;reserve_id=[% hold.reserve_id | html %]&amp;date=[% hold.date | html %]">
                            <img src="[% interface | html %]/[% theme | html %]/img/go-up.png" alt="Go up" />
                        </a>

                        <a title="Move hold to top" href="request.pl?action=move&amp;where=top&amp;first_priority=[% first_priority | html %]&amp;last_priority=[% last_priority | html %]&amp;prev_priority=[% prev_priority | html %]&amp;next_priority=[% next_priority | html %]&amp;borrowernumber=[% hold.borrowernumber | html %]&amp;biblionumber=[% hold.biblionumber | html %]&amp;reserve_id=[% hold.reserve_id | html %]&amp;date=[% hold.date | html %]">
                            <img src="[% interface | html %]/[% theme | html %]/img/go-top.png" alt="Go top" />
                        </a>

                        <a title="Move hold to bottom" href="request.pl?action=move&amp;where=bottom&amp;first_priority=[% first_priority | html %]&amp;last_priority=[% last_priority | html %]&amp;prev_priority=[% prev_priority | html %]&amp;next_priority=[% next_priority | html %]&amp;borrowernumber=[% hold.borrowernumber | html %]&amp;biblionumber=[% hold.biblionumber | html %]&amp;reserve_id=[% hold.reserve_id | html %]&amp;date=[% hold.date | html %]">
                            <img src="[% interface | html %]/[% theme | html %]/img/go-bottom.png" alt="Go bottom" />
                        </a>

                        <a title="Move hold down" href="request.pl?action=move&amp;where=down&amp;first_priority=[% first_priority | html %]&amp;last_priority=[% last_priority | html %]&amp;prev_priority=[% prev_priority | html %]&amp;next_priority=[% next_priority | html %]&amp;borrowernumber=[% hold.borrowernumber | html %]&amp;biblionumber=[% hold.biblionumber | html %]&amp;reserve_id=[% hold.reserve_id | html %]&amp;date=[% hold.date | html %]">
                            <img src="[% interface | html %]/[% theme | html %]/img/go-down.png" alt="Go down" />
                        </a>
                    </td>
        [% ELSE %]
            <td></td>
        [% END %]
            [% END %]

            <td>
                [% INCLUDE 'patron-title.inc' patron=hold.patron hide_patron_infos_if_needed=1 %]
            </td>

            <td>[% hold.notes | html %]</td>
            <td>
                [% IF Koha.Preference('AllowHoldDateInFuture') %]
                    <input type="text" class="datepicker" value="[% hold.date | $KohaDates %]" required="required" size="10" name="reservedate" />
                [% ELSE %]
                    [% hold.date | $KohaDates %]
                [% END %]
            </td>
            <td><input type="text" class="datepicker" value="[% hold.expirationdate | $KohaDates %]" size="10" name="expirationdate" /></td>

            <td>
                [% IF ( hold.found ) %]
                    [% IF ( hold.atdestination ) %]
                        [% IF ( hold.found ) %]
                            Item waiting at <b> [% hold.wbrname | html %]</b> <input type="hidden" name="pickup" value="[% hold.wbrcode | html %]" /> since [% hold.waiting_date | $KohaDates %]
                        [% ELSE %]
                            Waiting to be pulled <input type="hidden" name="pickup" value="[% hold.wbrcode | html %]" />
                        [% END %]
                    [% ELSE %]
                        Item being transferred to <b> [% hold.wbrname | html %]</b> <input type="hidden" name="pickup" value="[% hold.wbrcode | html %]" />
                    [% END %]
                [% ELSE %]
                    [% IF Koha.Preference('IndependentBranches') && Branches.all().size == 1 %]
                        [% Branches.GetName(hold.branchcode) | html %] <input type="hidden" name="pickup" value="[% hold.branchcode | html %]" />
                    [% ELSE %]
                        <select name="pickup">
                            [% PROCESS options_for_libraries libraries => Branches.all( selected => hold.branchcode, search_params => { "-or" => { pickup_location => 1, branchcode => hold.branchcode } } ) %]
                        </select>
                    [% END %]
                [% END %]
            </td>

            <td>
                [% IF ( hold.found ) %]
                    <a href="/cgi-bin/koha/catalogue/moredetail.pl?biblionumber=[% hold.biblionumber | uri %]&amp;itemnumber=[% hold.itemnumber | uri %]#item[% hold.itemnumber | uri %]">
                        [% IF ( hold.barcodenumber ) %]
                            [% hold.barcodenumber | html %]
                            <input type="hidden" name="itemnumber" value="[% hold.itemnumber | html %]" />
                        [% ELSE %]
                            No barcode
                        [% END %]
                    </a>
                [% ELSE %]
                    [% IF ( hold.item_level_hold ) %]
                        <i>
                            Only item
                            <a href="/cgi-bin/koha/catalogue/moredetail.pl?biblionumber=[% hold.biblionumber | uri %]&amp;itemnumber=[% hold.itemnumber | uri %]#item[% hold.itemnumber | uri %]">
                                [% IF ( hold.barcodenumber ) %]
                                    [% hold.barcodenumber | html %]
                                    <input type="hidden" name="itemnumber" value="[% hold.itemnumber | html %]" />
                                [% ELSE %]
                                    No barcode
                                [% END %]
                            </a>
                        </i>
                    [% ELSE %]
                        [% IF hold.itemtype %]
                            <i>Next available [% ItemTypes.GetDescription( hold.itemtype ) | html %] item</i>
                        [% ELSE %]
                            <i>Next available</i>
                        [% END %]

                        <input type="hidden" name="itemnumber" value="" />
                    [% END %]
                [% END %]
            </td>

            [% IF ( CAN_user_reserveforothers_modify_holds_priority ) %]
            [% UNLESS hold.found %]
                    <td>
                        <a title="Toggle lowest priority" href="request.pl?action=setLowestPriority&amp;borrowernumber=[% hold.borrowernumber | html %]&amp;biblionumber=[% hold.biblionumber | html %]&amp;reserve_id=[% hold.reserve_id | html %]&amp;date=[% hold.date | html %]">
                            [% IF ( hold.lowestPriority ) %]
                                <img src="[% interface | html %]/[% theme | html %]/img/go-bottom.png" alt="Unset lowest priority" />
                            [% ELSE %]
                                <img src="[% interface | html %]/[% theme | html %]/img/go-down.png" alt="Set to lowest priority" />
                            [% END %]
                        </a>
                    </td>
        [% ELSE %]
            <td></td>
        [% END %]
            [% END %]

            <td>
                <a class="cancel-hold" title="Cancel hold" href="request.pl?action=cancel&amp;borrowernumber=[% hold.borrowernumber | html %]&amp;biblionumber=[% hold.biblionumber | html %]&amp;reserve_id=[% hold.reserve_id | html %]&amp;date=[% hold.date | html %]">
                    <img src="[% interface | html %]/[% theme | html %]/img/x.png" alt="Cancel" />
                </a>
            </td>

            <td>
                [% IF Koha.Preference('SuspendHoldsIntranet') %]
                    [% UNLESS ( hold.found ) %]
                        <input type="button" value="[% IF ( hold.suspend ) %]Unsuspend[% ELSE %]Suspend[% END %]" onclick="window.location.href='request.pl?action=toggleSuspend&amp;reserve_id=[% hold.reserve_id | html %]&amp;borrowernumber=[% hold.borrowernumber | html %]&amp;biblionumber=[% hold.biblionumber | html %]&amp;date=[% hold.date | html %]&amp;suspend_until=' + $('#suspend_until_[% hold.reserve_id | html %]').val()" />

                        [% IF Koha.Preference('AutoResumeSuspendedHolds') %]
                            <label for="suspend_until_[% hold.reserve_id | html %]">[% IF ( hold.suspend ) %] on [% ELSE %] until [% END %]</label>
                            <input type="text" name="suspend_until" id="suspend_until_[% hold.reserve_id | html %]" size="10" value="[% hold.suspend_until | $KohaDates %]" class="datepicker suspend_until_datepicker" />
                            <a href='#' onclick="document.getElementById('suspend_until_[% hold.reserve_id | html %]').value='';">Clear date</a>
                        [% ELSE %]
                            <input type="hidden" name="suspend_until" id="suspend_until_[% hold.reserve_id | html %]" value=""/>
                        [% END %]
                    [% END %]
                [% END # IF SuspendHoldsIntranet %]

                [% IF ( hold.found ) %]
                    <input type="button" id="revert_hold_[% hold.reserve_id | html %]" value="[% IF hold.intransit %]Revert transit status[% ELSE %]Revert waiting status[% END %]" onclick="window.location.href='request.pl?action=move&amp;where=down&amp;first_priority=[% first_priority | uri %]&amp;last_priority=[% last_priority | uri %]&amp;prev_priority=0&amp;next_priority=1&amp;borrowernumber=[% hold.borrowernumber | uri %]&amp;biblionumber=[% hold.biblionumber | uri %]&amp;itemnumber=[% hold.itemnumber | uri %]&amp;reserve_id=[% hold.reserve_id | uri %]&amp;date=[% hold.date | uri %]'">
                [% END %]
            </td>

        </tr>

    [% END %]
</table>
